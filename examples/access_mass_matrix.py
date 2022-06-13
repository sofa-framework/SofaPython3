# Required import for python
import Sofa
import SofaRuntime
from Sofa import SofaLinearSolver
from scipy import sparse
from scipy import linalg
from matplotlib import pyplot as plt
import numpy as np

exportCSV = False
showImage = True

# Function called when the scene graph is being created
def createScene(root):

    root.addObject('VisualStyle', displayFlags="showBehaviorModels showForceFields")

    root.addObject('DefaultAnimationLoop')
    root.addObject('DefaultVisualManagerLoop')

    root.addObject('EulerImplicitSolver', rayleighStiffness="0.1", rayleighMass="0.1")
    root.addObject('SparseLDLSolver', applyPermutation="false", template="CompressedRowSparseMatrixd")

    root.addObject('MechanicalObject', name="DoFs")
    mass = root.addObject('MeshMatrixMass', name="mass", totalMass="320")
    root.addObject('RegularGridTopology', name="grid", nx="4", ny="4", nz="20", xmin="-9", xmax="-6", ymin="0", ymax="3", zmin="0", zmax="19")
    root.addObject('BoxROI', name="box", box="-10 -1 -0.0001  -5 4 0.0001")
    root.addObject('FixedConstraint', indices="@box.indices")
    root.addObject('HexahedronFEMForceField', name="FEM", youngModulus="4000", poissonRatio="0.3", method="large")

    root.addObject(MatrixAccessController('MatrixAccessor', name='matrixAccessor', mass=mass))

    return root


class MatrixAccessController(Sofa.Core.Controller):


    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.mass = kwargs.get("mass")

    def onAnimateEndEvent(self, event):
        mass_matrix = self.mass.assembleMMatrix()

        print('====================================')
        print('Stiffness matrix')
        print('====================================')
        print('dtype: ' + str(mass_matrix.dtype))
        print('shape: ' + str(mass_matrix.shape))
        print('ndim: ' + str(mass_matrix.ndim))
        print('nnz: ' + str(mass_matrix.nnz))
        print('norm: ' + str(sparse.linalg.norm(mass_matrix)))

        if exportCSV:
            np.savetxt('mass.csv', mass_matrix.toarray(), delimiter=',')
        if showImage:
            plt.imshow(mass_matrix.toarray(), interpolation='nearest', cmap='gist_gray')
            plt.show()

