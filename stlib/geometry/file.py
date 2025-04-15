from stlib.geometry import GeometryParameters
from splib.topology.loader import loadMesh
from Sofa.Core import Node

class FileParameters(GeometryParameters):
    def __init__(self,  filename, dynamicTopology = False):

        customGeom = FileParameters.createData(filename)
        GeometryParameters.__init__(data = customGeom, dynamicTopology = dynamicTopology)

    @staticmethod
    def createData(filename) -> GeometryParameters.Data :
        data = GeometryParameters.Data()
        
        node = Sofa.Core.Node()
        loadMesh(node, filename)
        node.init()

        data.positions = node.loader.position.value
        data.edges = node.loader.edges.value
        data.triangles = node.loader.triangles.value
        data.tetrahedra = node.loader.tetras.value

        node.cleanUp()

        return data
        