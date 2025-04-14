from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters, Callable, Optional, dataclasses, Any
from stlib.geometry import GeometryParameters
from Sofa.Core import Object 

@dataclasses.dataclass
class CollisionParameters(BaseParameters):
    primitives : list[Any] #TODO: add enum type in splib

    geometry : GeometryParameters
    addMapping : Callable


class Collision(BasePrefab):

    def __init__(self, params: GeometryParameters):
        BasePrefab.__init__(self, params)

