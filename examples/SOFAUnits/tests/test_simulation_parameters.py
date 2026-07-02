"""
Unit tests for SimulationParameters.py

Run with:  pytest test_simulation_parameters.py -v
"""
import pytest

from units import s, m, mm, ms, kg, N, Pa, kPa, kN, tho
from SimulationParameters import BaseParameterSet, SOFAParameters


# ---------------------------------------------------------------------------
# BaseParameterSet construction
# ---------------------------------------------------------------------------

class TestBaseParameterSetConstruction:
    def test_positional_primary_units_are_registered(self):
        bp = BaseParameterSet(s, m, kg)
        assert set(bp.units.keys()) == {"s", "m", "kg"}
        assert bp.units["s"] is s
        assert bp.units["m"] is m
        assert bp.units["kg"] is kg

    def test_keyword_primary_units_are_registered(self):
        bp = BaseParameterSet(time=s, length=mm, mass=kg)
        assert set(bp.units.keys()) == {"s", "m", "kg"}
        assert bp.units["m"] is mm

    def test_non_unit_positional_args_are_ignored(self):
        bp = BaseParameterSet(s, "not a unit", 42)
        assert set(bp.units.keys()) == {"s"}

    def test_non_unit_keyword_args_are_ignored(self):
        bp = BaseParameterSet(time=s, other="not a unit")
        assert set(bp.units.keys()) == {"s"}

    def test_derived_unit_positional_arg_raises_type_error(self):
        with pytest.raises(TypeError):
            BaseParameterSet(N)

    def test_derived_unit_keyword_arg_raises_type_error(self):
        with pytest.raises(TypeError):
            BaseParameterSet(force=N)

    def test_empty_construction_gives_empty_units(self):
        bp = BaseParameterSet()
        assert bp.units == {}


# ---------------------------------------------------------------------------
# BaseParameterSet.convert()
# ---------------------------------------------------------------------------

class TestConvert:
    def test_convert_with_all_base_units_matching(self):
        bp = BaseParameterSet(s, m, kg)
        assert bp.convert(10, N) == pytest.approx(10.0)

    def test_convert_scales_with_position_unit(self):
        # Simulation uses millimeters for position: 1 "simulation force unit"
        # corresponds to 1000 N because of the mm scaling.
        bp = BaseParameterSet(s, mm, kg)
        assert bp.convert(10, N) == pytest.approx(10000.0)

    def test_convert_pressure_with_base_units(self):
        bp = BaseParameterSet(s, m, kg)
        assert bp.convert(10, kPa) == pytest.approx(10000.0)

    def test_convert_pressure_with_mm_position(self):
        bp = BaseParameterSet(s, mm, kg)
        assert bp.convert(10, kPa) == pytest.approx(10.0)

    def test_convert_raises_when_required_unit_missing(self):
        bp = BaseParameterSet(s, kg)  # no 'm' defined
        with pytest.raises(RuntimeError):
            bp.convert(1, N)

    def test_convert_raises_mentions_missing_unit_abrev(self):
        bp = BaseParameterSet(s, kg)
        with pytest.raises(RuntimeError, match="m"):
            bp.convert(1, N)


# ---------------------------------------------------------------------------
# SOFAParameters
# ---------------------------------------------------------------------------

class TestSOFAParameters:
    def test_defaults_are_s_m_kg(self):
        sp = SOFAParameters()
        assert sp.units['s'] is s
        assert sp.units['m'] is m
        assert sp.units['kg'] is kg
        assert set(sp.units.keys()) == {"s", "m", "kg"}

    def test_custom_units_are_stored_as_attributes(self):
        sp = SOFAParameters(time=s, length=mm, mass=kg)
        assert sp.units['m']  is mm
        assert set(sp.units.keys()) == {"s", "m", "kg"}

    def test_convert_uses_configured_units(self):
        sp = SOFAParameters(time=s, length=mm, mass=kg)
        assert sp.convert(10, N) == pytest.approx(10000.0)
        assert sp.convert(10, kPa) == pytest.approx(10.0)

    def test_convert_with_default_units(self):
        sp = SOFAParameters()
        assert sp.convert(10, N) == pytest.approx(10.0)
        assert sp.convert(10, kPa) == pytest.approx(10000.0)



class TestConvertExponents:


    def test_convert_force_with_scaled_time_unit(self):
        bp = BaseParameterSet(ms, m, kg)
        # 1 sim force unit = 1 kg * 1 m / (1 ms)^2 = 1e6 N
        # -> converting 1 N (SI) into sim units should give 1e-6
        assert bp.convert(1, N) == pytest.approx(1e-6)


    def test_convert_torque_with_scaled_length_unit(self):
        bp = BaseParameterSet(s, mm, kg)
        # 1 sim torque unit = 1 kg * (1 mm)^2 / (1 s)^2 = 1e-6 tho
        # -> converting 1 tho (SI) into sim units should give 1e6
        assert bp.convert(1, tho) == pytest.approx(1e6)

    def test_convert_when_scaled_dimension_only_has_exponent_one(self):
        # Sanity check / contrast case: this is why the bug doesn't show
        # up anywhere in SimulationParameters.py as shipped - position is
        # scaled (mm) but only ever appears as m^1 in N and Pa, so the
        # missing exponent handling never bites here.
        bp = BaseParameterSet(s, mm, kg)
        assert bp.convert(10, N) == pytest.approx(10000.0)
        assert bp.convert(10, kPa) == pytest.approx(10.0)


if __name__ == "__main__":
    import sys
    sys.exit(pytest.main([__file__, "-v"]))
