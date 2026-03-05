from stlib.geometries import GeometryParameters
from stlib.geometries.file import FileParameters
import dataclasses


@dataclasses.dataclass
class CubeParameters(FileParameters):

    filename : str = "mesh/cube.obj"
