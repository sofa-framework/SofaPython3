import Sofa
import Sofa.Core
import numpy as np

class NaiveRestShapeSpringsForcefield(Sofa.Core.ForceField):
    def __init__(self, *args, **kwargs):
        Sofa.Core.ForceField.__init__(self, *args, **kwargs)
        self.addData(name="stiffness", value=kwargs.get("stiffness"), type="double",
                     help="scalar value representing the stiffness between"
                          "the actual position and the rest shape position")
        self.addData(name="rest_pos", type="vector<Vec3d>", value=kwargs.get("rest_pos"), default=[[]])
        self.addData(name="indices", type="vector<int>", value=kwargs.get("indices"), default=[])

    def init(self):
        if not self.rest_pos.isSet():
            print("ERROR: rest_pos required")
        if not self.indices.isSet():
            print ("Indices not set.")
            indices = [ i for i in list(range(len(self.rest_pos.value)))]
            self.indices = [ i for i in list(range(len(self.rest_pos.value)))]
            print (        [ i for i in list(range(len(self.rest_pos.value)))])
            print ("setting to " + str(self.indices.value))

    def addForce(self, m, forces, pos, vel):
        if self.rest_pos.hasChanged():
            self.init()
        print('addForce')
        k = self.stiffness.value
        with forces.writeable() as f:
            for index in self.indices.value:
                f[index] -= (pos[index] - self.rest_pos.value[index]) * k


    def addDForce(self, m, dforce, dx):
        print('addDForce')
        # kFactorIncludingRayleighDamping -> assuming rayleighStiffness of 0.0
        # kF = mparams['kFactor'] + mparams['bFactor'] * self.rayleighStiffness.value
        k = self.stiffness.value * m['kFactor']
        with dforce.writeable() as df:
            for index in self.indices.value:
                df[index] -= dx[index] * k

    def _addKToMatrix_selectivePoints(self, mparams, nNodes, nDofs):
        K = []

        # kFactorIncludingRayleighDamping => rayleighStiffness = 0.0
        kF = mparams['kFactor'] + mparams['bFactor'] * 0.0

        for index in self.indices.value:
            for n in range(0, nDofs):
                K.append([nDofs * index + n, nDofs * index +
                          n, -kF * self.stiffness.value])
        return np.asarray(K)

    def _addKToMatrix_plainMatrix(self, mparams, nNodes, nDofs):
        K = np.zeros((nNodes * nDofs, nNodes * nDofs, 1), dtype=float)
        # kFactorIncludingRayleighDamping => rayleighStiffness = 0.0
        kF = mparams['kFactor'] + mparams['bFactor'] * 0.0

        for idx in self.indices.value:
            for n in range(0, nDofs):
                K[nDofs * idx + n, nDofs * idx + n] = -kF * self.stiffness.value
        return K

    def addKToMatrix(self, mparams, nNodes, nDofs):
        if self.rest_pos.hasChanged():
            self.init()
        print("addKToMatrix")
        return self._addKToMatrix_selectivePoints(mparams, nNodes, nDofs)
        #return self._addKToMatrix_plainMatrix(mparams, nNodes, nDofs)




def MyRestShapeSpringsForcefield(*args, **kwargs):
    #    for key, value in kwargs.items():
    #        if key == "implementation":
    #            if value == "naive":
    return NaiveRestShapeSpringsForcefield(args, kwargs)
    #            if value == "vectorized":
    #                return VectorizedRestShapeSpringsForcefield(args, kwargs)
    #            if value == "numpy":
    #                return NumpyRestShapeSpringsForcefield(args, kwargs)
    #            if value == "DataContainer":
    #                return DataContainerRestShapeSpringsForcefield(args, kwargs)
