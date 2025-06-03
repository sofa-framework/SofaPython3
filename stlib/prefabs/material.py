from stlib.core.baseParameters import BaseParameters, Callable, Optional, dataclasses, Any
from splib.core.enum_types import *
from stlib.core.basePrefab import BasePrefab
from splib.mechanics.mass import addMass

@dataclasses.dataclass
class MaterialParameters(BaseParameters):
    name: str = "Material"

    massDensity: float = 0
    massLumping: bool = False
    
    stateType: StateType = StateType.VEC3

    addMaterial : Optional[Callable] = lambda node : addMass(node, node.stateType, massDensity=node.massDensity, lumping=node.massLumping)


# TODO : previously called Behavior
class Material(BasePrefab):
    def __init__(self, params: MaterialParameters):
        BasePrefab.__init__(self, params)

        self.params = params

        print(params.stateType.toString())
        self.addObject("MechanicalObject", name="States", template=str(self.params.stateType))

        if(params.addMaterial is not None):
            params.addMaterial(self)
