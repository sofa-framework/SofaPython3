"""Constitutive models for soft robotics materials.

Each model documents the strain energy expression SOFA uses.

Nomenclature:
    E: Young's modulus (Pa)
    μ: small-strain shear modulus (Pa)
    ν: Poisson's ratio
    K: bulk modulus (Pa)
"""

from typing import NamedTuple

from Sofa.Units.Core import DimensionnedValue
from Sofa.Units.Definitions import DimensionLess, Pa
from Sofa.Units.UnitSystem import MechanicalUnitSystem


SI = MechanicalUnitSystem()


def K(mu, nu, spatial_dimension):
    """Bulk modulus of an isotropic material, K = 2μ(1 + (d − 2)ν) / (d(1 − 2ν)), where d is the spatial dimension."""
    return 2 * mu * (1 + (spatial_dimension - 2) * nu) / (spatial_dimension * (1 - 2 * nu))


class LinearElasticMaterial(NamedTuple):
    """W = μ ε:ε + λ/2 (tr ε)², where ε is the small strain in the element's rotated frame and λ is Lamé's first parameter"""
    E: DimensionnedValue

    def to_sofa_data(self, nu, spatial_dimension):
        return {"youngModulus": self.E, "poissonRatio": nu * DimensionLess}


class MooneyRivlinMaterial(NamedTuple):
    """W = C10(Ī₁ − 3) + C01(Ī₂ − 3) + K/2 (ln J)²"""
    C10: DimensionnedValue
    C01: DimensionnedValue
    C20: DimensionnedValue  # not in SOFA's energy

    def to_sofa_data(self, nu, spatial_dimension):
        # In 2D, Ī₂ is constant. C01 does not contribute.
        mu = 2 * (SI(self.C10) + SI(self.C01)) * Pa if spatial_dimension == 3 else 2 * self.C10
        return {"mu10": self.C10, "mu01": self.C01, "bulkModulus": K(mu, nu, spatial_dimension)}


class NeoHookeanMaterial(NamedTuple):
    """W = μ/2 (I₁ − 3) − μ ln J + λ/2 (ln J)², where λ is Lamé's first parameter"""
    mu: DimensionnedValue

    def to_sofa_data(self, nu, spatial_dimension):
        return {"youngModulus": 2 * self.mu * (1 + nu), "poissonRatio": nu * DimensionLess}


class OgdenMaterial(NamedTuple):
    """W = Σ μ_p/α_p² (λ̄₁^α_p + λ̄₂^α_p + λ̄₃^α_p − 3) + K/2 (ln J)²"""
    mu_p: tuple[DimensionnedValue, ...]
    alpha_p: tuple[DimensionnedValue, ...]

    def to_sofa_data(self, nu, spatial_dimension):
        mu = sum(SI(mu_p) for mu_p in self.mu_p) / 2 * Pa
        return {"mu": list(self.mu_p), "alpha": list(self.alpha_p), "kappa": K(mu, nu, spatial_dimension)}


# TODO: SOFA has no Yeoh component yet
class YeohMaterial(NamedTuple):
    """W = Σ C_i(Ī₁ − 3)^i + K/2 (ln J)²"""
    C1: DimensionnedValue
    C2: DimensionnedValue
    C3: DimensionnedValue

    def to_sofa_data(self, nu, spatial_dimension):
        return {"C1": self.C1, "C2": self.C2, "C3": self.C3, "bulkModulus": K(2 * self.C1, nu, spatial_dimension)}
