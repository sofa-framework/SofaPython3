from stlib.geometries import GeometryParameters, InternalDataProvider
import dataclasses
import numpy as np

@dataclasses.dataclass
class PlaneDataProvider(InternalDataProvider):
    center : np.ndarray[float]
    normal : np.ndarray[float]
    lengthNormal : np.ndarray[float]
    nbEdgeLength : int
    nbEdgeWidth : int
    width : float
    edgeSize : float

    def __post_init__(self, **kwargs):
        InternalDataProvider.__init__(self,**kwargs)

    def generateAttribute(self):
        #TODO
        pass


class PlaneParameters(GeometryParameters):

    def __init__(self,  center, normal, lengthNormal, length, width, nbEdgeLength, nbEdgeWidth, dynamicTopology = False):
        GeometryParameters.__init__(data = PlaneDataProvider(center=center, normal=normal, lengthNormal=lengthNormal, length=length, width=width, nbEdgeLength=nbEdgeLength, nbEdgeWidth=nbEdgeWidth),
                                    dynamicTopology = dynamicTopology)
