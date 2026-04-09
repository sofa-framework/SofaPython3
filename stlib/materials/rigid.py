from stlib.materials import MaterialParameters
from splib.core.enum_types import StateType


class RigidParameters(MaterialParameters):

    stateType : StateType = StateType.RIGID
