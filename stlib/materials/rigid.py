from stlib.core.baseParameters import Optional, dataclasses
from stlib.materials import MaterialParameters
from splib.core.utils import DEFAULT_VALUE
from splib.core.enum_types import StateType


@dataclasses.dataclass
class RigidParameters(MaterialParameters):
    
    stateType : StateType = StateType.RIGID

