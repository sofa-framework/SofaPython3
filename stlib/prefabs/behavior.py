from stlib.core.baseParameters import BaseParameters, Callable, Optional, dataclasses, Any
from splib.core.enum_types import StateType


@dataclasses.dataclass
class BehaviorParameters(BaseParameters):
    name: str = "Behavior"

    stateType: StateType = StateType.VEC3
    massDensity: float # TODO : discuss with Hugo massDensity/totalMass
    


    

