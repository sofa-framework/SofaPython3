import Sofa
from Sofa.Units import Definitions
from Sofa.Units.UnitSystem import MechanicalUnitSystem

from . import library, models


class Material(Sofa.Prefab):
    prefabParameters = (
        {"name": "material", "type": "string", "help": "Name of the material", "default": ""},
        {"name": "model", "type": "string", "help": "Constitutive model of the material", "default": "LinearElastic"},
        {"name": "poissonRatio", "type": "double", "help": "Poisson's ratio, defaults to the material's"},
        {"name": "template", "type": "string", "help": "Template of the force field", "default": "Vec3,Tetrahedron"},
        {"name": "units", "type": "string", "help": "Time, length and mass units of the scene", "default": "s m kg"},
    )

    def __init__(self, *args, forceFieldData=None, **kwargs):
        """forceFieldData: Data of the force field to set"""
        if "poissonRatio" not in kwargs:
            kwargs["poissonRatio"] = models.SI(library.get_material(kwargs.get("material", "")).nu)
        super().__init__(*args, **kwargs)

        for name, value in (forceFieldData or {}).items():
            self.forcefield.findData(name).value = value

    def init(self):
        material = library.get_material(self.material.value)
        constitutive_model = getattr(material, self.model.value, None)
        if constitutive_model is None:
            raise ValueError(f"{self.model.value} has not been implemented for {material.name}")

        state_template = self.template.value.split(",")[0]
        spatial_dimension = int(state_template[3])

        time, length, mass = (getattr(Definitions, symbol) for symbol in self.units.value.split())
        units = MechanicalUnitSystem(time, length, mass)

        sofa_data = {}
        for name, value in constitutive_model.to_sofa_data(self.poissonRatio.value, spatial_dimension).items():
            sofa_data[name] = units(value)

        if isinstance(constitutive_model, models.LinearElasticMaterial):
            self.addObject("CorotationalFEMForceField"
                           , name="forcefield"
                           , template=self.template.value
                           , **sofa_data
            )
            return

        self.addObject(type(constitutive_model).__name__
                       , name="constitutiveModel"
                       , template=state_template
                       , **sofa_data
        )
        self.addObject("HyperelasticityFEMForceField"
                       , name="forcefield"
                       , template=self.template.value
                       , material="@constitutiveModel"
        )
