"""Constitutive model parameters for soft robotics materials.

Each material documents the strain energy expressions used in its reference and converts the reference's parameters
to those of SOFA's models.

Nomenclature:
    E: Young's modulus (Pa)
    μ: small-strain shear modulus (Pa)
    ν: Poisson's ratio
    K: bulk modulus (Pa)
"""

from Sofa.Units.Definitions import DimensionLess, Pa

from .models import (
    LinearElasticMaterial,
    MooneyRivlinMaterial,
    NeoHookeanMaterial,
    OgdenMaterial,
    YeohMaterial,
)


class DragonSkin20:
    """L. Marechal, P. Balland, L. Lindenroth, F. Petrou, C. Kontovounisios and F. Bello, "Toward a Common Framework and
    Database of Materials for Soft Robotics," Soft Robotics, 2020, doi: 10.1089/soro.2019.0115.

    Neo-Hookean: W = μ(I₁ − 3)
    Mooney-Rivlin: W = C10(I₁ − 3) + C01(I₂ − 3) + C20(I₁ − 3)²
    Yeoh: W = Σ C_i(I₁ − 3)^i
    Ogden: W = Σ μ_p/α_p (λ₁^α_p + λ₂^α_p + λ₃^α_p − 3)
    """

    name = "Dragon Skin 20"
    nu = 0.49 * DimensionLess
    LinearElastic = LinearElasticMaterial(E=4e5 * Pa)
    MooneyRivlin = MooneyRivlinMaterial(C10=2.47e5 * Pa, C01=-3.74e5 * Pa, C20=38.8 * Pa)
    NeoHookean = NeoHookeanMaterial(mu=2 * 2.07047e5 * Pa)
    Ogden = OgdenMaterial(mu_p=(3.478 * -9.534e5 * Pa, 3.181 * -1.4515e6 * Pa, 3.339 * 2.4085e6 * Pa), alpha_p=(3.478 * DimensionLess, 3.181 * DimensionLess, 3.339 * DimensionLess))
    Yeoh = YeohMaterial(C1=1e5 * Pa, C2=1.69e5 * Pa, C3=266 * Pa)


class DragonSkin30:
    """L. Marechal, P. Balland, L. Lindenroth, F. Petrou, C. Kontovounisios and F. Bello, "Toward a Common Framework and
    Database of Materials for Soft Robotics," Soft Robotics, 2020, doi: 10.1089/soro.2019.0115.

    Neo-Hookean: W = μ(I₁ − 3)
    Mooney-Rivlin: W = C10(I₁ − 3) + C01(I₂ − 3) + C20(I₁ − 3)²
    Yeoh: W = Σ C_i(I₁ − 3)^i
    Ogden: W = Σ μ_p/α_p (λ₁^α_p + λ₂^α_p + λ₃^α_p − 3)
    """

    name = "Dragon Skin 30"
    nu = 0.49 * DimensionLess
    LinearElastic = LinearElasticMaterial(E=4e5 * Pa)
    MooneyRivlin = MooneyRivlinMaterial(C10=2.47e5 * Pa, C01=-3.30e5 * Pa, C20=209 * Pa)
    NeoHookean = NeoHookeanMaterial(mu=2 * 2.4203e5 * Pa)
    Ogden = OgdenMaterial(mu_p=(3.417 * 3.816e4 * Pa, 3.417 * 2.524e4 * Pa, 3.417 * 4.456e4 * Pa), alpha_p=(3.417 * DimensionLess, 3.417 * DimensionLess, 3.417 * DimensionLess))
    Yeoh = YeohMaterial(C1=1e5 * Pa, C2=1.19e5 * Pa, C3=604 * Pa)


class Ecoflex0010:
    """L. Marechal, P. Balland, L. Lindenroth, F. Petrou, C. Kontovounisios and F. Bello, "Toward a Common Framework and
    Database of Materials for Soft Robotics," Soft Robotics, 2020, doi: 10.1089/soro.2019.0115.

    Neo-Hookean: W = μ(I₁ − 3)
    Mooney-Rivlin: W = C10(I₁ − 3) + C01(I₂ − 3) + C20(I₁ − 3)²
    Yeoh: W = Σ C_i(I₁ − 3)^i
    Ogden: W = Σ μ_p/α_p (λ₁^α_p + λ₂^α_p + λ₃^α_p − 3)
    """

    name = "Ecoflex 00-10"
    nu = 0.49 * DimensionLess
    LinearElastic = LinearElasticMaterial(E=4e4 * Pa)
    MooneyRivlin = MooneyRivlinMaterial(C10=2.18e4 * Pa, C01=-7.22e4 * Pa, C20=12.6 * Pa)
    NeoHookean = NeoHookeanMaterial(mu=2 * 2.264e4 * Pa)
    Ogden = OgdenMaterial(mu_p=(2.858 * 7.363e5 * Pa, 2.604 * 8.074e5 * Pa, 2.740 * -1.526e6 * Pa), alpha_p=(2.858 * DimensionLess, 2.604 * DimensionLess, 2.740 * DimensionLess))
    Yeoh = YeohMaterial(C1=1e3 * Pa, C2=1.31e4 * Pa, C3=20.56 * Pa)


class Ecoflex0030:
    """L. Marechal, P. Balland, L. Lindenroth, F. Petrou, C. Kontovounisios and F. Bello, "Toward a Common Framework and
    Database of Materials for Soft Robotics," Soft Robotics, 2020, doi: 10.1089/soro.2019.0115.

    Neo-Hookean: W = μ(I₁ − 3)
    Mooney-Rivlin: W = C10(I₁ − 3) + C01(I₂ − 3) + C20(I₁ − 3)²
    Yeoh: W = Σ C_i(I₁ − 3)^i
    Ogden: W = Σ μ_p/α_p (λ₁^α_p + λ₂^α_p + λ₃^α_p − 3)
    """

    name = "Ecoflex 00-30"
    nu = 0.49 * DimensionLess
    LinearElastic = LinearElasticMaterial(E=1e5 * Pa)
    MooneyRivlin = MooneyRivlinMaterial(C10=1.8e4 * Pa, C01=-4.69e4 * Pa, C20=43.4 * Pa)
    NeoHookean = NeoHookeanMaterial(mu=2 * 3.245e4 * Pa)
    Ogden = OgdenMaterial(mu_p=(3.310 * -3.22e5 * Pa, 3.115 * 1.9e5 * Pa, 3.468 * 1.45e5 * Pa), alpha_p=(3.310 * DimensionLess, 3.115 * DimensionLess, 3.468 * DimensionLess))
    Yeoh = YeohMaterial(C1=1e5 * Pa, C2=1.2e4 * Pa, C3=49.6 * Pa)


class Pebax2533:
    name = "Pebax 2533"
    nu = 0.4 * DimensionLess
    LinearElastic = LinearElasticMaterial(E=1.2e7 * Pa)


class Pebax4033:
    name = "Pebax 4033"
    nu = 0.4 * DimensionLess
    LinearElastic = LinearElasticMaterial(E=7.3e7 * Pa)


class Pebax6333:
    name = "Pebax 6333"
    nu = 0.4 * DimensionLess
    LinearElastic = LinearElasticMaterial(E=3e8 * Pa)


class Pebax7233:
    name = "Pebax 7233"
    nu = 0.4 * DimensionLess
    LinearElastic = LinearElasticMaterial(E=5.1e8 * Pa)


class PLA:
    """A. Ouyoucef, Q. Peyron, G. Zheng and F. Boyer, "Modeling, analysis and design of an extensible planar parallel
    tendon actuated continuum robot," 2025 IEEE 8th International Conference on Soft Robotics (RoboSoft), 2025,
    doi: 10.1109/RoboSoft63089.2025.11020851.
    """

    name = "PLA"
    nu = 0.3 * DimensionLess
    LinearElastic = LinearElasticMaterial(E=3e9 * Pa)


class TPU:
    name = "TPU"
    nu = 0.45 * DimensionLess
    LinearElastic = LinearElasticMaterial(E=1e8 * Pa)


MATERIALS = {material.name: material for material in (
    DragonSkin20,
    DragonSkin30,
    Ecoflex0010,
    Ecoflex0030,
    Pebax2533,
    Pebax4033,
    Pebax6333,
    Pebax7233,
    PLA,
    TPU,
)}


def get_material(name):
    material = MATERIALS.get(name)
    if material is None:
        raise ValueError(f"Unknown material '{name}'. Available materials: {', '.join(MATERIALS)}")
    return material
