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

        if hasattr(parent.loader, 'position'):
            self.position = str(parent.loader.position.linkpath)
        if hasattr(parent.loader, 'edges'):
            self.edges = str(parent.loader.edges.linkpath)
        if hasattr(parent.loader, 'triangles'):
            self.triangles = str(parent.loader.triangles.linkpath)
        if hasattr(parent.loader, 'quads'):
            self.quads = str(parent.loader.quads.linkpath)
        if hasattr(parent.loader, 'hexahedra'):
            self.hexahedra = str(parent.loader.hexahedra.linkpath)
        if hasattr(parent.loader, 'tetrahedra'):
            self.tetrahedra = str(parent.loader.tetrahedra.linkpath)



class FileParameters(GeometryParameters):

    def __init__(self, filename, dynamicTopology = False, elementType : ElementType = None ):
        GeometryParameters.__init__(self,
                                    data = FileInternalDataProvider(filename=filename), 
                                    dynamicTopology = dynamicTopology, 
                                    elementType = elementType)
        
