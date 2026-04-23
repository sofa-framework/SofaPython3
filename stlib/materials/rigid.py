from stlib.materials import MaterialParameters
from splib.core.enum_types import StateType


class RigidMaterialParameters(MaterialParameters):

    stateType : StateType = StateType.RIGID

    position : list[list[float]] = [[0., 0., 0., 0., 0., 0., 1.]]
