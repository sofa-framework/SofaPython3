from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters, Optional, dataclasses
from stlib.prefabs.geometries import Geometry, GeometryParameters
from stlib.prefabs.geometries.file import FileParameters
from splib.core.utils import DEFAULT_VALUE
from Sofa.Core import Object 

@dataclasses.dataclass
class VisualParameters(BaseParameters):
    name : str = "Visual"

    color : Optional[list[float]] = DEFAULT_VALUE
    texture :  Optional[str] = DEFAULT_VALUE

    geometry : GeometryParameters = dataclasses.field(default_factory = lambda : GeometryParameters())


class Visual(BasePrefab):

    def __init__(self, parameters: VisualParameters):
        BasePrefab.__init__(self, parameters)

    def init(self):
        self.geometry = self.add(Geometry, self.parameters.geometry)
        self.addObject("OglModel", color=self.parameters.color, src=self.geometry.container.linkpath)


    @staticmethod
    def getParameters(**kwargs) -> VisualParameters:
        return VisualParameters(**kwargs)


def createScene(root):

    # Create a visual from a mesh file
    parameters = Visual.getParameters() 
    parameters.name = "LiverVisual"
    parameters.geometry = FileParameters(filename="mesh/liver.obj")
    root.add(Visual, parameters)