from stlib.geometries import GeometryParameters, InternalDataProvider, Geometry
from stlib.core.baseParameters import dataclasses
from splib.topology.loader import loadMesh
from splib.core.enum_types import ElementType

from Sofa.Core import Node

@dataclasses.dataclass
class FileInternalDataProvider(InternalDataProvider):
    filename : str = "mesh/cube.obj"

    def __post_init__(self, **kwargs):
        InternalDataProvider.__init__(self,**kwargs)

    def generateAttribute(self, parent : Geometry):    
        loadMesh(parent, self.filename)      

        self.position = str(parent.loader.position.linkpath)
        self.edges = str(parent.loader.edges.linkpath)
        self.triangles = str(parent.loader.triangles.linkpath)
        self.quads = str(parent.loader.quads.linkpath)
        self.hexahedra = str(parent.loader.hexahedra.linkpath)
        self.tetrahedra = str(parent.loader.tetras.linkpath)



class FileParameters(GeometryParameters):

    def __init__(self, filename, dynamicTopology = False, elementType : ElementType = None ):
        GeometryParameters.__init__(self,
                                    data = FileInternalDataProvider(filename=filename), 
                                    dynamicTopology = dynamicTopology, 
                                    elementType = elementType)
        
