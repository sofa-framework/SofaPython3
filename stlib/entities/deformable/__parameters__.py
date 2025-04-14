from stlib.core.baseParameters import BaseParameters, Callable, Optional, dataclasses
from stlib.geometry import GeometryParameters
from stlib.prefabs.visual import VisualParameters
from stlib.prefabs.collision import CollisionParameters

@dataclasses.dataclass
class DeformableParameters(BaseParameters):     

    geometry : GeometryParameters
    # Add default value
    visual  : VisualParameters 
    collision  : CollisionParameters
    mass : float = float  

    addConstitutiveLaw : Callable = lambda x: x

    def toDict(self):
        return dataclasses.asdict(self)
