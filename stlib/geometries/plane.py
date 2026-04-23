from stlib.geometries import GeometryParameters, InternalDataProvider, Geometry
import numpy as np

class PlaneDataProvider(InternalDataProvider):
    center : np.ndarray[float] = np.array([0,0,0])
    normal : np.ndarray[float] = np.array([0,0,1])
    lengthNormal : np.ndarray[float] = np.array([1,0,0])
    lengthNbEdge : int = 1
    widthNbEdge : int = 1
    lengthSize : float = 1.0
    widthSize : float = 1.0

    def generateAttribute(self, parent : Geometry):

        lengthEdgeSize = self.lengthSize / self.lengthNbEdge
        widthEdgeSize = self.widthSize / self.widthNbEdge

        self.widthNormal = np.cross(self.normal,self.lengthNormal)
        bottomLeftCorner = self.center - self.lengthNormal * self.lengthNbEdge * lengthEdgeSize / 2 - self.widthNormal * self.widthNbEdge * widthEdgeSize / 2

        self.position = np.array([[ bottomLeftCorner + j * self.widthNormal * widthEdgeSize + i * self.lengthNormal * lengthEdgeSize   for j in range(self.widthNbEdge + 1) ] for i in range(self.lengthNbEdge + 1)])

        self.triangles = np.empty((2 * self.widthNbEdge * self.lengthNbEdge, 3), dtype = int)
        for i in range(self.lengthNbEdge):
            for j in range(self.widthNbEdge):
                self.triangles[i*self.widthNbEdge*2 + j * 2    , 0] = j     + i     * (self.widthNbEdge + 1)
                self.triangles[i*self.widthNbEdge*2 + j * 2    , 1] = j     + (i+1) * (self.widthNbEdge + 1)
                self.triangles[i*self.widthNbEdge*2 + j * 2    , 2] = j + 1 + i     * (self.widthNbEdge + 1)
                self.triangles[i*self.widthNbEdge*2 + j * 2 + 1, 0] = j + 1 + i     * (self.widthNbEdge + 1)
                self.triangles[i*self.widthNbEdge*2 + j * 2 + 1, 1] = j     + (i+1) * (self.widthNbEdge + 1)
                self.triangles[i*self.widthNbEdge*2 + j * 2 + 1, 2] = j + 1 + (i+1) * (self.widthNbEdge + 1)



class PlaneParameters(GeometryParameters):

    center: list[float, float, float] = [0, 0, 0]
    normal: list[float, float, float] = [0, 0, 1]
    lengthNormal: list[float, float, float] = [1, 0, 0]
    lengthNbEdge: int = 1
    widthNbEdge: int = 1
    lengthSize: float = 1.0
    widthSize: float = 1.0
    dynamicTopology: bool = False

    def model_post_init(self, __context):
        self.data = PlaneDataProvider(center=self.center, 
                                      normal=self.normal, 
                                      lengthNormal=self.lengthNormal, 
                                      lengthNbEdge=self.lengthNbEdge, 
                                      widthNbEdge=self.widthNbEdge, 
                                      lengthSize=self.lengthSize, 
                                      widthSize=self.widthSize)
