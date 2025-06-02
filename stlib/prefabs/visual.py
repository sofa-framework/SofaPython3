from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters, Callable, Optional, dataclasses, Any
from stlib.geometry import Geometry, GeometryParameters
from stlib.geometry.extract import ExtractParameters
from stlib.geometry.file import FileParameters
from splib.core.enum_types import ElementType
from Sofa.Core import Object 

@dataclasses.dataclass
class VisualParameters(BaseParameters):
    color : Optional[list[float]] = None
    texture :  Optional[str] = None

    geometry : GeometryParameters = dataclasses.field(default_factory = lambda : GeometryParameters())
    addMapping : Optional[Callable] = None


class Visual(BasePrefab):
    def __init__(self, params: VisualParameters):
        BasePrefab.__init__(self, params)

        geom = self.add(Geometry, params.geometry)
        # TODO : handle optional color, texture using DEFAULT_VALUE mechanism (as implemented by Paul)
        self.addObject("OglModel", color=params.color, src=geom.container.linkpath)

        if params.addMapping is not None:
            params.addMapping(self)

    @staticmethod
    def getParameters(**kwargs) -> VisualParameters:
        return VisualParameters(**kwargs)


def createScene(root):

    # Create a visual from a mesh file
    params = Visual.getParameters() 
    params.name = "VisualFromFile"
    params.geometry = FileParameters(filename="mesh/cube.obj")
    root.add(Visual, params)

    # # Create a visual from a node 
    # params = Visual.getParameters()  
    # params.name = "ExtractedVisual"
    # params.geometry = ExtractParameters(sourceParameters=FileParameters(filename="mesh/cube.vtk"), 
    #                                     destinationType=ElementType.TRIANGLES)
    # root.add(Visual, params)
