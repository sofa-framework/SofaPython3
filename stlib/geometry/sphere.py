from stlib.geometry import GeometryParameters

class SphereParameters(GeometryParameters):
    def __init__(self,  center, radius, pointPerRad, dynamicTopology = False):

        customGeom = SphereParameters.createData(center, radius, pointPerRad)
        GeometryParameters.__init__(data = customGeom, dynamicTopology = dynamicTopology)

    @staticmethod
    def createData(center, radius, pointPerRad) -> GeometryParameters.Data :
        data = GeometryParameters.Data()
        #Fill data
        return data
        