from stlib.geometry import GeometryParameters, InternalDataProvider, Geometry
from stlib.core.baseParameters import dataclasses
from splib.topology.loader import loadMesh
from splib.core.enum_types import ElementType

from Sofa.Core import Node

@dataclasses.dataclass
class FileInternalDataProvider(InternalDataProvider):
    filename : str

    def __post_init__(self, **kwargs):
        InternalDataProvider.__init__(self,**kwargs)

    def generateAttribute(self, parent : Geometry):        
    
        loadMesh(parent, self.filename)

        self.positions = parent.loader.position.linkpath
        self.edges = parent.loader.edges.linkpath
        self.triangles = parent.loader.triangles.linkpath
        self.quads = parent.loader.quads.linkpath
        self.hexaedra = parent.loader.hexaedra.linkpath
        self.tetrahedra = parent.loader.tetras.linkpath



class FileParameters(GeometryParameters):
    def __init__(self,  filename, dynamicTopology = False, elementType : ElementType = None ):
        GeometryParameters.__init__(data = FileInternalDataProvider(filename), dynamicTopology = dynamicTopology, elementType = elementType)
        
