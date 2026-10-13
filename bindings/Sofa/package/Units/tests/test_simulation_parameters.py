"""
Unit tests for UnitSystem.py

Run with:  pytest test_simulation_parameters.py -v
"""
import pytest

from Units.Definitions import s, m, mm, ms, kg, N, Pa, kPa, kN, tau
from Units.UnitSystem import UnitSystem, MechanicalUnitSystem


# ---------------------------------------------------------------------------
# UnitSystem construction
# ---------------------------------------------------------------------------

class TestUnitSystemConstruction:
    def test_positional_primary_units_are_registered(self):
        bp = UnitSystem(s, m, kg)
        assert set(bp.units.keys()) == {"s", "m", "kg"}
        assert bp.units["s"] is s
        assert bp.units["m"] is m
        assert bp.units["kg"] is kg

    def test_keyword_primary_units_are_registered(self):
        bp = UnitSystem(time=s, length=mm, mass=kg)
        assert set(bp.units.keys()) == {"s", "m", "kg"}
        assert bp.units["m"] is mm

    def test_non_unit_positional_args_are_ignored(self):
        bp = UnitSystem(s, "not a unit", 42)
        assert set(bp.units.keys()) == {"s"}

    def test_non_unit_keyword_args_are_ignored(self):
        bp = UnitSystem(time=s, other="not a unit")
        assert set(bp.units.keys()) == {"s"}

    def test_derived_unit_positional_arg_raises_type_error(self):
        with pytest.raises(TypeError):
            UnitSystem(N)

    def test_derived_unit_keyword_arg_raises_type_error(self):
        with pytest.raises(TypeError):
            UnitSystem(force=N)

    def test_empty_construction_gives_empty_units(self):
        bp = UnitSystem()
        assert bp.units == {}


# ---------------------------------------------------------------------------
# UnitSystem.convert()
# ---------------------------------------------------------------------------

class TestConvert:
    def test_convert_with_all_base_units_matching(self):
        bp = UnitSystem(s, m, kg)
        assert bp.convert(10, N) == pytest.approx(10.0)

    def test_convert_scales_with_position_unit(self):
        # Simulation uses millimeters for position: 1 "simulation force unit"
        # corresponds to 1000 N because of the mm scaling.
        bp = UnitSystem(s, mm, kg)
        assert bp.convert(10, N) == pytest.approx(10000.0)

    def test_convert_pressure_with_base_units(self):
        bp = UnitSystem(s, m, kg)
        assert bp.convert(10, kPa) == pytest.approx(10000.0)

    def test_convert_pressure_with_mm_position(self):
        bp = UnitSystem(s, mm, kg)
        assert bp.convert(10, kPa) == pytest.approx(10.0)

    def test_convert_raises_when_required_unit_missing(self):
        bp = UnitSystem(s, kg)  # no 'm' defined
        with pytest.raises(RuntimeError):
            bp.convert(1, N)

    def test_convert_raises_mentions_missing_unit_abrev(self):
        bp = UnitSystem(s, kg)
        with pytest.raises(RuntimeError, match="m"):
            bp.convert(1, N)


# ---------------------------------------------------------------------------
# MechanicalUnitSystem
# ---------------------------------------------------------------------------

class TestMechanicalUnitSystem:
    def test_defaults_are_s_m_kg(self):
        sp = MechanicalUnitSystem()
        assert sp.units['s'] is s
        assert sp.units['m'] is m
        assert sp.units['kg'] is kg
        assert set(sp.units.keys()) == {"s", "m", "kg"}

    def test_custom_units_are_stored_as_attributes(self):
        sp = MechanicalUnitSystem(time=s, length=mm, mass=kg)
        assert sp.units['m']  is mm
        assert set(sp.units.keys()) == {"s", "m", "kg"}

    def test_convert_uses_configured_units(self):
        sp = MechanicalUnitSystem(time=s, length=mm, mass=kg)
        assert sp.convert(10, N) == pytest.approx(10000.0)
        assert sp.convert(10, kPa) == pytest.approx(10.0)

    def test_convert_with_default_units(self):
        sp = MechanicalUnitSystem()
        assert sp.convert(10, N) == pytest.approx(10.0)
        assert sp.convert(10, kPa) == pytest.approx(10000.0)



class TestConvertExponents:


    def test_convert_force_with_scaled_time_unit(self):
        bp = UnitSystem(ms, m, kg)
        # 1 sim force unit = 1 kg * 1 m / (1 ms)^2 = 1e6 N
        # -> converting 1 N (SI) into sim units should give 1e-6
        assert bp.convert(1, N) == pytest.approx(1e-6)


    def test_convert_torque_with_scaled_length_unit(self):
        bp = UnitSystem(s, mm, kg)
        # 1 sim torque unit = 1 kg * (1 mm)^2 / (1 s)^2 = 1e-6 tau
        # -> converting 1 tau (SI) into sim units should give 1e6
        assert bp.convert(1, tau) == pytest.approx(1e6)

    def test_convert_when_scaled_dimension_only_has_exponent_one(self):
        # Sanity check / contrast case: this is why the bug doesn't show
        # up anywhere in SimulationParameters.py as shipped - position is
        # scaled (mm) but only ever appears as m^1 in N and Pa, so the
        # missing exponent handling never bites here.
        bp = UnitSystem(s, mm, kg)
        assert bp.convert(10, N) == pytest.approx(10000.0)
        assert bp.convert(10, kPa) == pytest.approx(10.0)


# ===========================================================================
# ADDITIONAL COVERAGE (added 2026-07-03)
# ===========================================================================

import numpy as np
from Units.Definitions import g, v, DimensionLess


class TestDuplicateAndMixedRegistration:
    def test_duplicate_primary_unit_raises_value_error(self):
        with pytest.raises(ValueError):
            UnitSystem(s, s)

    def test_scaled_and_plain_of_same_dimension_raises(self):
        # mm and m share the abbreviation "m" -> second one must be refused
        with pytest.raises(ValueError):
            UnitSystem(m, mm)

    def test_scaled_unit_is_accepted_as_primary(self):
        bp = UnitSystem(mm)
        assert bp.units["m"] is mm


class TestConvertMoreCases:
    def test_convert_scaled_target_unit_uses_its_ratio(self):
        bp = UnitSystem(s, m, kg)
        assert bp.convert(1, kN) == pytest.approx(1000.0)

    def test_convert_scaled_target_against_matching_scaled_base(self):
        bp = UnitSystem(s, mm, kg)
        assert bp.convert(1, mm) == pytest.approx(1.0)

    def test_convert_with_scaled_mass_unit(self):
        # base mass = gram: 1 sim force unit = 1 g*m/s^2 = 1e-3 N
        bp = UnitSystem(s, m, g)
        assert bp.convert(1, N) == pytest.approx(1e3)

    def test_convert_dimensionless_is_identity(self):
        bp = UnitSystem(s, m, kg)
        assert bp.convert(5, DimensionLess) == pytest.approx(5.0)

    def test_convert_missing_denominator_unit_raises(self):
        bp = UnitSystem(m, kg)  # no 's' defined, v = m/s needs it
        with pytest.raises(RuntimeError, match="s"):
            bp.convert(1, v)

    def test_convert_zero_value(self):
        bp = UnitSystem(s, mm, kg)
        assert bp.convert(0, N) == pytest.approx(0.0)

    def test_convert_negative_value(self):
        bp = UnitSystem(s, mm, kg)
        assert bp.convert(-1, N) == pytest.approx(-1000.0)


class TestCallDispatch:
    """__call__ accepts (DimensionnedValue) or (float, Unit), scalars,
    lists and numpy arrays."""

    def setup_method(self):
        self.bp = UnitSystem(s, mm, kg)  # 1 N -> 1000 sim units

    def test_call_with_dimensionned_value(self):
        assert self.bp(10 * N) == pytest.approx(10000.0)

    def test_call_with_float_and_unit(self):
        assert self.bp(10, N) == pytest.approx(10000.0)

    def test_call_with_list_of_floats_and_unit(self):
        assert self.bp([1.0, 2.0], N) == pytest.approx([1000.0, 2000.0])

    def test_call_with_nested_list(self):
        assert self.bp([[1.0], [2.0]], N) == [[pytest.approx(1000.0)], [pytest.approx(2000.0)]]

    def test_call_with_ndarray_and_unit(self):
        out = self.bp(np.array([1.0, 2.0]), N)
        assert isinstance(out, np.ndarray)
        assert list(out) == pytest.approx([1000.0, 2000.0])

    def test_call_with_ndarray_of_dimensionned_values(self):
        arr = np.array([1 * N, 2 * N], dtype=object)
        out = self.bp(arr)
        assert list(out) == pytest.approx([1000.0, 2000.0])

    def test_call_with_list_of_dimensionned_values(self):
        assert self.bp([1 * N, 2 * N]) == pytest.approx([1000.0, 2000.0])

    def test_call_output_array_is_float32(self):
        # NOTE/quirk: converted arrays are allocated as float32, so large
        # magnitudes silently lose precision (~7 significant digits).
        out = self.bp(np.array([1.0]), N)
        assert out.dtype == np.float32

    def test_call_with_no_args_raises_value_error(self):
        with pytest.raises(ValueError):
            self.bp()

    def test_call_with_three_args_raises_value_error(self):
        with pytest.raises(ValueError):
            self.bp(1.0, N, N)


class TestMechanicalUnitSystemMore:
    def test_positional_override(self):
        sp = MechanicalUnitSystem(ms, mm)
        assert sp.units["s"] is ms
        assert sp.units["m"] is mm
        assert sp.units["kg"] is kg

    def test_mixed_scaled_units_conversion(self):
        # time in ms, length in mm: 1 sim force = kg*mm/ms^2 = 1000 N
        sp = MechanicalUnitSystem(time=ms, length=mm)
        assert sp.convert(1, N) == pytest.approx(1e-3)


if __name__ == "__main__":
    import sys
    sys.exit(pytest.main([__file__, "-v"]))
