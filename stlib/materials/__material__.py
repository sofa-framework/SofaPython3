from stlib.core.baseParameters import BaseParameters, Callable, Optional, Any
from splib.core.utils import DEFAULT_VALUE
from splib.core.enum_types import StateType

from stlib.core.basePrefab import BasePrefab
from splib.mechanics.mass import addMass


class MaterialParameters(BaseParameters):
    name : str = "Material"

    position : Optional[list[float]] = None

    massDensity : float = DEFAULT_VALUE
    massLumping : bool = DEFAULT_VALUE

    stateType : StateType = StateType.VEC3

    addMaterial : Optional[Callable] = lambda node : addMass(node, 
                                                             elementType=None, 
                                                             massDensity=node.parameters.massDensity, 
                                                             lumping=node.parameters.massLumping)


class Material(BasePrefab):

    parameters : MaterialParameters

    def __init__(self, parameters: MaterialParameters):
        BasePrefab.__init__(self, parameters)

    def init(self):
        self.addObject("MechanicalObject", name="States", template=str(self.parameters.stateType),
                       position = self.parameters.position if self.parameters.position is not None else "")
        self.parameters.addMaterial(self)
