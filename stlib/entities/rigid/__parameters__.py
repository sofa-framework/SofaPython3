from stlib.core.baseParameters import BaseParameters, Callable, Optional, dataclasses
from stlib.geometry import GeometryParameters

@dataclasses.dataclass
class RigidParameters(BaseParameters):     

    geometry : GeometryParameters
    mass : Optional[float] = None    

    def toDict(self):
        return dataclasses.asdict(self)
