# Required import for python
import Sofa
import SofaRuntime
from scipy import sparse
from scipy import linalg
from matplotlib import pyplot as plt
import numpy as np

exportCSV = False
showImage = True

# Function called when the scene graph is being created
def createScene(root):

    root.addObject('VisualStyle', displayFlags="showBehaviorModels showForceFields")

    root.addObject("RequiredPlugin", pluginName=['Sofa.Component.Constraint.Projective',
    'Sofa.Component.Engine.Select',
    'Sofa.Component.LinearSolver.Direct',
    'Sofa.Component.Mass',
    'Sofa.Component.ODESolver.Backward',
    'Sofa.Component.SolidMechanics.FEM.Elastic',
    'Sofa.Component.StateContainer',
    'Sofa.Component.Topology.Container.Grid',
    'Sofa.Component.Visual'
    ])

    root.addObject('DefaultAnimationLoop')
    root.addObject('DefaultVisualManagerLoop')

    root.addObject('EulerImplicitSolver', rayleighStiffness="0.1", rayleighMass="0.1")
    root.addObject('SparseLDLSolver', applyPermutation="false", template="CompressedRowSparseMatrixd")

    root.addObject('MechanicalObject', name="DoFs")
    root.addObject('MeshMatrixMass', name="mass", totalMass="320")
    root.addObject('RegularGridTopology', name="grid", nx="4", ny="4", nz="20", xmin="-9", xmax="-6", ymin="0", ymax="3", zmin="0", zmax="19")
    root.addObject('BoxROI', name="box", box="-10 -1 -0.0001  -5 4 0.0001")
    root.addObject('FixedConstraint', indices="@box.indices")
    FEM = root.addObject('HexahedronFEMForceField', name="FEM", youngModulus="4000", poissonRatio="0.3", method="large")

    root.addObject(MatrixAccessController('MatrixAccessor', name='matrixAccessor', force_field=FEM))

    return root


class MatrixAccessController(Sofa.Core.Controller):


    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.force_field = kwargs.get("force_field")

    def onAnimateEndEvent(self, event):
        stiffness_matrix = self.force_field.assembleKMatrix()

        print('====================================')
        print('Stiffness matrix')
        print('====================================')
        print('dtype: ' + str(stiffness_matrix.dtype))
        print('shape: ' + str(stiffness_matrix.shape))
        print('ndim: ' + str(stiffness_matrix.ndim))
        print('nnz: ' + str(stiffness_matrix.nnz))
        print('norm: ' + str(sparse.linalg.norm(stiffness_matrix)))

        if exportCSV:
            np.savetxt('stiffness.csv', stiffness_matrix.toarray(), delimiter=',')
        if showImage:
            plt.imshow(stiffness_matrix.toarray(), interpolation='nearest', cmap='gist_gray')
            plt.show(block=False)

