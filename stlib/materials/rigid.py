from stlib.core.baseParameters import BaseParameters, Optional, dataclasses
from stlib.geometries import GeometryParameters



@dataclasses.dataclass
class RigidParameters(BaseParameters):

    geometry : GeometryParameters
    mass : Optional[float] = None

    def toDict(self):
        return dataclasses.asdict(self)
