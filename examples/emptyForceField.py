import Sofa.Core
import numpy as np
from Sofa.Helper import msg_info

class EmptyForcefield(Sofa.Core.ForceField):
    def __init__(self, *args, **kwargs):
        Sofa.Core.ForceField.__init__(self, *args, **kwargs)
        pass

    def init(self):
        pass

    def addForce(self, m, forces, pos, vel):
        msg_info('Not implemented yet')
        pass

    def addDForce(self, m, dforce, dx):
        msg_info('Not implemented yet')
        pass

    def addKToMatrix(self, mparams, nNodes, nDofs):
        msg_info('Not implemented yet')
        pass

