from stlib.geometries import GeometryParameters, InternalDataProvider, Geometry
from splib.topology.loader import loadMesh
from splib.core.enum_types import ElementType

from Sofa.Core import Node


class FileParameters(GeometryParameters):

    filename : str = "mesh/cube.obj"
    dynamicTopology : bool = False
    elementType : ElementType = None

    translation : list[float, float, float] = [0., 0., 0.]
    rotation : list[float, float, float] = [0., 0., 0.]
    scale : list[float, float, float] = [1., 1., 1.]

    def model_post_init(self, __context):
        self.data = FileInternalDataProvider(fileParameters=self)

class FileInternalDataProvider(InternalDataProvider):

    fileParameters : FileParameters

    def generateAttribute(self, parent : Geometry):    
        loader = loadMesh(parent, self.fileParameters.filename)
        loader.translation = self.fileParameters.translation
        loader.rotation = self.fileParameters.rotation
        loader.scale = self.fileParameters.scale

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

        return parent.loader
