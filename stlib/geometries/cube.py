from stlib.geometries import GeometryParameters

class CubeParameters(GeometryParameters):
    def __init__(self,  center, edgeLength, pointPerEdge, dynamicTopology = False):

        customGeom = CubeParameters.createData(center, edgeLength, pointPerEdge)
        GeometryParameters.__init__(data = customGeom, dynamicTopology = dynamicTopology)

    @staticmethod
    def createData(center, edgeLength, pointPerEdge) -> GeometryParameters.Data :
        data = GeometryParameters.Data()
        #Fill data
        return data
        