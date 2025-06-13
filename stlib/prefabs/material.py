from stlib.core.baseParameters import BaseParameters, Callable, Optional, dataclasses, Any
from splib.core.utils import defaultValueType, DEFAULT_VALUE, isDefault
from splib.core.enum_types import StateType

from stlib.core.basePrefab import BasePrefab
from splib.mechanics.mass import addMass

@dataclasses.dataclass
class MaterialParameters(BaseParameters):
    name : str = "Material"

    massDensity : float = DEFAULT_VALUE
    massLumping : bool = DEFAULT_VALUE
    
    stateType : StateType = StateType.VEC3

    addMaterial : Optional[Callable] = lambda node : addMass(node, node.parameters.stateType, massDensity=node.parameters.massDensity, lumping=node.parameters.massLumping)


# TODO : previously called Behavior
class Material(BasePrefab):

    parameters : MaterialParameters

    def __init__(self, parameters: MaterialParameters):
        BasePrefab.__init__(self, parameters)
        self.parameters = parameters

        self.addObject("MechanicalObject", name="States", template=str(parameters.stateType))
        parameters.addMaterial(self)
