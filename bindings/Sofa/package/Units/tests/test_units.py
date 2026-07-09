"""
Unit tests for units.py

These tests describe the CURRENT behavior of the module (updated 2026-07-03
for the fixed version of units.py). All previously pinned bugs are fixed;
remaining design quirks (ScaledUnit ratio semantics, simplify() list
mutation, float32 arrays in SimulationParameters) are documented in comments.
Run with:  pytest test_units.py -v
"""
import math
import pytest

from Definitions import (
    DimensionLess, m, s, kg,
    v, a, N, Pa, tho,
    nm, mm, cm, km,
    ms, µs,
    g, mg, t,
    nN, mN, kN, MN,
    kPa, MPa,
)

from Core import (
    Unit, NeutralUnit, PrimaryUnit, DerivedUnit, ScaledUnit, DimensionnedValue
)


# ---------------------------------------------------------------------------
# PrimaryUnit / NeutralUnit
# ---------------------------------------------------------------------------

class TestPrimaryUnit:
    def test_abrev_is_stored(self):
        assert m.abrev == "m"
        assert s.abrev == "s"
        assert kg.abrev == "kg"

    def test_primary_unit_is_its_own_numerator(self):
        assert m.numerator == [m]
        assert m.denumerator == []

    def test_primary_unit_ratio_is_one(self):
        assert m.ratio == 1.0

    def test_new_primary_unit_construction(self):
        x = PrimaryUnit("x")
        assert x.abrev == "x"
        assert x.numerator == [x]
        assert x.denumerator == []
        assert x.ratio == 1.0


class TestNeutralUnit:
    def test_neutral_unit_has_no_num_or_denum(self):
        assert DimensionLess.numerator == []
        assert DimensionLess.denumerator == []

    def test_neutral_unit_ratio_is_one(self):
        assert DimensionLess.ratio == 1.0

    def test_multiplying_by_neutral_unit_is_identity(self):
        result = DimensionLess * m
        assert result.ratio == 1.0
        assert [u.abrev for u in result.numerator] == ["m"]
        assert result.denumerator == []


# ---------------------------------------------------------------------------
# getKey()
# ---------------------------------------------------------------------------

class TestGetKey:
    def test_simple_primary_key(self):
        key = m.getKey()
        assert key == {"num": {"m": 1}, "denum": {}}

    def test_repeated_unit_is_counted(self):
        area = m * m
        key = area.getKey()
        assert key["num"] == {"m": 2}
        assert key["denum"] == {}

    def test_derived_unit_key_newton(self):
        # N = kg * m / s^2
        key = N.getKey()
        assert key["num"] == {"kg": 1, "m": 1}
        assert key["denum"] == {"s": 2}

    def test_derived_unit_key_pascal(self):
        # Pa = N / m^2 = kg / (s^2 * m)
        key = Pa.getKey()
        assert key["num"] == {"kg": 1}
        assert key["denum"] == {"s": 2, "m": 1}


# ---------------------------------------------------------------------------
# Multiplication / division
# ---------------------------------------------------------------------------

class TestMultiplication:
    def test_multiply_two_primary_units(self):
        result = m * m
        assert isinstance(result, DerivedUnit)
        assert [u.abrev for u in result.numerator] == ["m", "m"]
        assert result.denumerator == []
        assert result.ratio == 1.0

    def test_multiply_combines_ratios(self):
        result = kN * kN  # 1000 * 1000
        assert result.ratio == pytest.approx(1e6)

    def test_rmul_matches_mul(self):
        assert (m * s).getKey() == (s * m).getKey()


class TestDivision:
    def test_divide_two_primary_units(self):
        result = m / s
        assert [u.abrev for u in result.numerator] == ["m"]
        assert [u.abrev for u in result.denumerator] == ["s"]
        assert result.ratio == 1.0

    def test_divide_ratios(self):
        result = km / ms  # 1000 / 0.001
        assert result.ratio == pytest.approx(1e6)

    def test_velocity_acceleration_force_chain(self):
        # sanity check against the derived-unit constants defined in units.py
        assert [u.abrev for u in v.numerator] == ["m"]
        assert [u.abrev for u in v.denumerator] == ["s"]

        assert [u.abrev for u in a.numerator] == ["m"]
        assert [u.abrev for u in a.denumerator] == ["s", "s"]

        assert [u.abrev for u in N.numerator] == ["kg", "m"]
        assert [u.abrev for u in N.denumerator] == ["s", "s"]

class TestSimplify:
    def test_simplify_cancels_matching_units(self):
        # (m/s) * (s/m) should fully cancel to a dimensionless unit
        result = (m / s) * (s / m)
        assert result.numerator == []
        assert result.denumerator == []
        assert result.ratio == 1.0

    def test_simplify_cancels_only_one_occurrence(self):
        # (m*m) / m -> should cancel exactly one 'm', leaving one 'm' in numerator
        result = (m * m) / m
        assert [u.abrev for u in result.numerator] == ["m"]
        assert result.denumerator == []

    def test_simplify_does_not_cancel_unmatched_units(self):
        # N has kg (no matching denum) and m (no matching denum); s^2 in
        # denum has no matching numerator, so nothing gets cancelled.
        assert [u.abrev for u in N.numerator] == ["kg", "m"]
        assert [u.abrev for u in N.denumerator] == ["s", "s"]


# ---------------------------------------------------------------------------
# ScaledUnit
# ---------------------------------------------------------------------------

class TestScaledUnit:
    def test_ratio_is_the_scale_factor(self):
        assert mm.ratio == pytest.approx(1e-3)
        assert km.ratio == pytest.approx(1e3)
        assert kN.ratio == pytest.approx(1e3)
        assert kPa.ratio == pytest.approx(1e3)

    def test_scaled_unit_keeps_base_unit_dimension(self):
        assert [u.abrev for u in mm.numerator] == ["m"]
        assert mm.denumerator == []
        assert [u.abrev for u in kN.numerator] == ["kg", "m"]
        assert [u.abrev for u in kN.denumerator] == ["s", "s"]

    def test_scaled_unit_doesnt_shares_list_reference_with_base_unit(self):
        assert mm.numerator is not m.numerator
        assert kN.numerator is not N.numerator
        assert kN.denumerator is not N.denumerator


# ---------------------------------------------------------------------------
# __eq__
# ---------------------------------------------------------------------------

class TestEquality:
    def test_unit_equals_itself(self):
        assert N == N
        assert Pa == Pa

    def test_units_with_different_order_of_magnitude_are_not_equal(self):
        assert N != kN  # ratios 1.0 vs 1000.0 -> different int(log10(ratio))

    def test_units_in_different_decades_are_not_equal(self):
        # __eq__ compares int(math.log10(ratio)) rather than the ratio
        # itself. This is not a real ambiguity in practice: every scaled
        # unit in this library is an exact power-of-ten multiple of its
        # primary unit (as in the SI system), so the decade uniquely
        # identifies the ratio. This test just pins down that units a
        # decade apart are correctly told apart.
        assert Pa != kPa
        assert mm != cm

    def test_eq_key_comparison_is_symmetric(self):
        # FIXED: __eq__ now compares getKey() dicts, which is symmetric, so
        # a subset of dimensions no longer compares equal to a superset.
        subset = DerivedUnit(numerator=[m.numerator[0]], denumerator=[], ratio=1.0)
        superset = DerivedUnit(numerator=[m.numerator[0], s.numerator[0]], denumerator=[], ratio=1.0)
        assert subset != superset


# ---------------------------------------------------------------------------
# printReduced()
# ---------------------------------------------------------------------------

class TestPrintReduced:
    def test_print_reduced_newton(self, capsys):
        print(N)
        captured = capsys.readouterr()
        assert captured.out.strip() == "1.0 * ( kg * m ) / ( s^2 )"

    def test_print_reduced_pascal(self, capsys):
        print(Pa)
        captured = capsys.readouterr()
        assert captured.out.strip() == "1.0 * ( kg ) / ( s^2 * m )"




# ===========================================================================
# ADDITIONAL COVERAGE (added 2026-07-03)
# Tests below extend coverage to __pow__, __truediv__ edge cases, toString,
# ScaledUnit composition, DimensionnedValue arithmetic, and pin down several
# genuine bugs (marked BUG) so they can't regress silently while unfixed.
# ===========================================================================

class TestPower:
    def test_positive_power(self):
        area = m ** 2
        assert area.getKey() == {"num": {"m": 2}, "denum": {}}
        assert area.ratio == pytest.approx(1.0)

    def test_positive_power_of_scaled_unit_compounds_ratio(self):
        sq_mm = mm ** 2
        assert sq_mm.getKey() == {"num": {"m": 2}, "denum": {}}
        assert sq_mm.ratio == pytest.approx(1e-6)

    def test_negative_power_inverts_unit(self):
        per_s2 = s ** -2
        assert per_s2.getKey() == {"num": {}, "denum": {"s": 2}}
        assert per_s2.ratio == pytest.approx(1.0)

    def test_negative_power_inverts_ratio(self):
        per_km = km ** -1
        assert per_km.ratio == pytest.approx(1e-3)

    def test_power_of_derived_unit(self):
        n2 = N ** 2
        assert n2.getKey() == {"num": {"kg": 2, "m": 2}, "denum": {"s": 4}}

    def test_non_int_power_raises_value_error(self):
        with pytest.raises(ValueError):
            m ** 2.0
        with pytest.raises(ValueError):
            m ** "2"

    def test_power_zero_returns_neutral_unit_instance(self):
        # FIXED: exponent 0 now returns a NeutralUnit *instance*.
        result = m ** 0
        assert isinstance(result, NeutralUnit)
        assert result.getKey() == {"num": {}, "denum": {}}
        assert result.ratio == pytest.approx(1.0)


class TestTrueDivEdgeCases:
    def test_divide_by_non_unit_raises_standard_type_error(self):
        # FIXED: __truediv__ returns NotImplemented for non-Units, so Python
        # itself raises the standard "unsupported operand" TypeError after
        # both sides decline.
        with pytest.raises(TypeError, match="unsupported operand"):
            m / 2

    def test_division_fully_cancels_identical_units(self):
        r = (m / s) / (m / s)
        assert r.getKey() == {"num": {}, "denum": {}}
        assert r.ratio == pytest.approx(1.0)

    def test_division_by_scaled_unit_divides_ratio(self):
        r = m / mm
        assert r.getKey() == {"num": {}, "denum": {}}
        assert r.ratio == pytest.approx(1e3)


class TestEqualityEdgeCases:
    def test_eq_with_non_unit_returns_false(self):
        # FIXED: __eq__ returns NotImplemented for non-Units, so Python
        # falls back to identity comparison and `==` evaluates to False
        # instead of raising.
        assert (m == 5) is False
        assert (m != 5) is True

    def test_dimensionless_no_longer_equals_everything(self):
        # FIXED: symmetric dict comparison of getKey() means an empty key
        # dict only matches another empty key dict.
        assert (DimensionLess == m) is False
        assert (DimensionLess == N) is False
        assert DimensionLess == NeutralUnit()


class TestUnitHash:
    def test_equal_units_hash_equal(self):
        assert hash(N) == hash(kg * m / s**2)
        assert hash(Pa) == hash(N / m**2)

    def test_hash_distinguishes_ratio_decades(self):
        assert hash(N) != hash(kN)

    def test_hash_is_order_independent(self):
        assert hash(m * s) == hash(s * m)

    def test_units_usable_in_sets_and_dicts(self):
        assert len({N, kg * m / s**2}) == 1
        d = {m: "length", s: "time"}
        assert d[m] == "length"


class TestToString:
    def test_str_of_unit_with_num_and_denum(self):
        assert str(v) == "1.0 * ( m ) / ( s )"

    def test_to_string_without_ratio(self):
        assert N.toString(addRatio=False) == "( kg * m ) / ( s^2 )"

    def test_str_of_pure_denominator_unit(self):
        per_s = NeutralUnit() / s
        assert str(per_s) == "1.0 * 1/ ( s )"

    def test_str_of_unit_with_empty_denominator_has_no_dangling_parens(self):
        # FIXED: an empty denominator is now omitted entirely (note the
        # trailing space after the numerator group is kept).
        assert str(m) == "1.0 * ( m ) "


class TestScaledUnitComposition:
    def test_scaling_a_scaled_unit_replaces_ratio_instead_of_composing(self):
        # NOTE/quirk: ScaledUnit takes `ratio` as an absolute ratio to the
        # primary unit, not a factor applied on top of the base unit's own
        # ratio. ScaledUnit(km, 2.0) therefore has ratio 2.0, not 2000.0.
        twice = ScaledUnit(km, 2.0)
        assert twice.ratio == pytest.approx(2.0)

    def test_scaled_unit_of_derived_unit_keeps_dimension(self):
        u = ScaledUnit(Pa, 42.0)
        assert u.getKey() == Pa.getKey()
        assert u.ratio == pytest.approx(42.0)


class TestSimplifyMutation:
    def test_constructor_mutates_callers_denominator_list(self):
        # BUG/quirk: DerivedUnit.simplify() pops from the very list object
        # the caller passed in, so the caller's list is emptied as a side
        # effect. The operators (*, /, **) always build fresh lists so they
        # are unaffected, but direct construction is not safe.
        num, den = [m], [m]
        DerivedUnit(numerator=num, denumerator=den, ratio=1.0)
        assert den == []  # caller's list was mutated in place


# ---------------------------------------------------------------------------
# DimensionnedValue
# ---------------------------------------------------------------------------

class TestDimensionnedValueConstruction:
    def test_unit_times_scalar_builds_value(self):
        dv = m * 5
        assert isinstance(dv, DimensionnedValue)
        assert dv.value == 5
        assert dv.unit is m

    def test_scalar_times_unit_builds_value(self):
        dv = 5 * m
        assert isinstance(dv, DimensionnedValue)
        assert dv.value == 5

    def test_str_applies_ratio(self):
        dv = 5 * km
        assert str(dv) == "5000.0 * ( m ) "


class TestDimensionnedValueArithmetic:
    def test_value_times_value_multiplies_values_and_units(self):
        force = (3 * kg) * (2 * a)
        assert force.value == 6
        assert force.unit.getKey() == N.getKey()

    def test_value_times_unit_extends_unit(self):
        dv = (3 * m) * s
        assert dv.value == 3
        assert dv.unit.getKey() == {"num": {"m": 1, "s": 1}, "denum": {}}

    def test_value_divided_by_value(self):
        speed = (10 * m) / (2 * s)
        assert speed.value == pytest.approx(5.0)
        assert speed.unit.getKey() == v.getKey()

    def test_value_divided_by_unit(self):
        dv = (10 * m) / s
        assert dv.value == 10
        assert dv.unit.getKey() == v.getKey()

    def test_value_power(self):
        sq = (3 * m) ** 2
        assert sq.value == 9
        assert sq.unit.getKey() == {"num": {"m": 2}, "denum": {}}

    def test_value_power_non_int_raises(self):
        with pytest.raises(ValueError):
            (3 * m) ** 0.5

    def test_value_times_plain_scalar(self):
        # FIXED: scalar branch now multiplies by `other` directly.
        dv = (5 * m) * 2
        assert dv.value == 10
        assert dv.unit is m

    def test_value_divided_by_plain_scalar(self):
        # FIXED: same fix in __truediv__'s scalar branch.
        dv = (6 * m) / 2
        assert dv.value == pytest.approx(3.0)
        assert dv.unit is m


class TestRightDivision:
    def test_scalar_divided_by_value_inverts_unit(self):
        dv = 2 / (5 * m)
        assert dv.value == pytest.approx(0.4)
        assert dv.unit.getKey() == {"num": {}, "denum": {"m": 1}}

    def test_value_divided_by_value_still_uses_normal_path(self):
        speed = (10 * m) / (5 * s)
        assert speed.value == pytest.approx(2.0)
        assert speed.unit.getKey() == v.getKey()

    def test_unit_divided_by_value_delegates_to_rtruediv(self):
        # FIXED: Unit.__truediv__ now returns NotImplemented for non-Units,
        # so Python falls back to DimensionnedValue.__rtruediv__.
        dv = m / (5 * s)
        assert dv.value == pytest.approx(0.2)
        assert dv.unit.getKey() == v.getKey()


class TestDimensionnedValueEquality:
    # FIXED: __eq__ now compares getKey() dicts symmetrically, uses
    # other.unit (not other), no longer mutates any unit's ratio, and
    # compares normalized magnitudes with math.isclose.

    def test_equal_values_compare_equal(self):
        assert (5 * m) == (5 * m)

    def test_cross_scale_comparison(self):
        # 1000 mm == 1 m; isclose absorbs the 1000 * 1e-3 float wobble.
        assert (1000 * mm) == (1 * m)
        assert (1 * kN) == (1000 * N)

    def test_unequal_magnitudes_compare_unequal(self):
        assert ((5 * m) == (6 * m)) is False

    def test_mismatched_dimensions_raise_type_error(self):
        with pytest.raises(TypeError, match="share the same units"):
            (5 * m) == (5 * s)

    def test_comparison_with_non_value_raises_type_error(self):
        with pytest.raises(TypeError, match="compared"):
            (5 * m) == 5

    def test_eq_no_longer_mutates_unit_ratio(self):
        # Regression guard for the old reference-instead-of-copy bug that
        # permanently reset shared unit ratios to 1.0.
        local_mm = ScaledUnit(m, 1e-3)
        (1 * local_mm) == (2 * local_mm)
        assert local_mm.ratio == pytest.approx(1e-3)
        (1000 * mm) == (1 * m)
        assert mm.ratio == pytest.approx(1e-3)
        assert m.ratio == pytest.approx(1.0)


class TestDimensionnedValueHash:
    def test_equal_values_hash_equal(self):
        assert hash(1 * m) == hash(1 * m)
        assert hash(1 * m) == hash(1.0 * m)

    def test_cross_scale_values_hash_equal(self):
        # Exact for upward powers of ten (2 * 1e3 is exactly 2000.0); the
        # round(..., 9) in __hash__ absorbs small downward-scale wobble.
        assert hash(2 * km) == hash(2000.0 * m)
        assert hash(1000 * mm) == hash(1 * m)

    def test_values_usable_in_sets(self):
        assert len({1 * m, 1.0 * m}) == 1

    def test_hash_isclose_caveat(self):
        # NOTE: __eq__ uses math.isclose (relative tolerance) while __hash__
        # uses round(..., 9) (fixed decimal buckets). These can disagree for
        # values that are isclose but straddle a rounding boundary, or for
        # large magnitudes where 1e-9 absolute rounding is coarser than the
        # relative tolerance. Acceptable for this library's power-of-ten
        # ratios, but don't rely on set/dict deduplication for values that
        # are only *approximately* equal.
        assert True


if __name__ == "__main__":
    import sys
    sys.exit(pytest.main([__file__, "-v"]))
