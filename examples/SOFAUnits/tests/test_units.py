"""
Unit tests for units.py

These tests describe the CURRENT behavior of the module, including a couple
of quirks (documented in comments) that were not changed, per request.
Run with:  pytest test_units.py -v
"""
import math
import pytest

from units import (
    Unit, NeutralUnit, PrimaryUnit, DerivedUnit, ScaledUnit,
    DimensionLess, m, s, kg,
    v, a, N, Pa, tho,
    nm, mm, cm, km,
    ms, µs,
    g, mg, t,
    nN, mN, kN, MN,
    kPa, MPa,
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

    def test_eq_key_comparison_is_one_directional(self):
        # NOTE: __eq__ only checks that every key of `self` is present (and
        # matches) in `other`; it does not check the reverse. So an object
        # with a superset of keys on the "other" side can compare equal to
        # a subset on the "self" side, as long as ratio decade matches too.
        subset = DerivedUnit(numerator=[m.numerator[0]], denumerator=[], ratio=1.0)
        superset = DerivedUnit(numerator=[m.numerator[0], s.numerator[0]], denumerator=[], ratio=1.0)
        assert subset == superset


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


if __name__ == "__main__":
    import sys
    sys.exit(pytest.main([__file__, "-v"]))
