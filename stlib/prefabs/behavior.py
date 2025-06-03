from stlib.core.baseParameters import BaseParameters, Callable, Optional, dataclasses, Any
from splib.core.enum_types import StateType
from stlib.core.basePrefab import BasePrefab


@dataclasses.dataclass
class BehaviorParameters(BaseParameters):
    name: str = "Behavior"

    stateType: StateType = StateType.VEC3
    massDensity: float # TODO : discuss with Hugo massDensity/totalMass
    


    

class Behavior(BasePrefab):
    def __init__(self, params: BehaviorParameters):
        BasePrefab.__init__(self, params)
