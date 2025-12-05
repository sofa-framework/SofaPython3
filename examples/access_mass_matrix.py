# Required import for python
import Sofa
import SofaRuntime
from Sofa import SofaLinearSystem
from scipy import sparse
from scipy import linalg
from matplotlib import pyplot as plt
import numpy as np

exportCSV = False
showImage = True


def create_beam(node, id, offset):

    node.addObject('MechanicalObject', name="DoFs")
    mass = node.addObject('MeshMatrixMass', name="mass" + str(id), totalMass=320)
    node.addObject('RegularGridTopology', name="grid", nx=4, ny=4, nz=20, xmin=-9 + offset, xmax=-6 + offset, ymin=0, ymax=3, zmin=0, zmax=19)
    node.addObject('BoxROI', name="box", box=[-10 + offset, -1, -0.0001,  -5 + offset, 4, 0.0001])
    node.addObject('FixedProjectiveConstraint', indices="@box.indices")
    node.addObject('HexahedronFEMForceField', name="FEM", youngModulus=4000, poissonRatio=0.3, method="large")

    node.addObject(LocalMatrixAccessController('MatrixAccessor', name='matrixAccessor', mass=mass))

# Function called when the scene graph is being created
def createScene(root):

    root.addObject('VisualStyle', displayFlags="showBehaviorModels showForceFields")

    root.addObject("RequiredPlugin", pluginName=['Sofa.Component.Constraint.Projective',
                                                    'Sofa.Component.Engine.Select',
                                                    'Sofa.Component.LinearSolver.Direct',
                                                    'Sofa.Component.LinearSystem',
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

    matrices = root.addChild('matrices')
    # in this Node, two linear systems are declared:
    # 1) one used to assemble the global system used by the solver, and
    # 2) another assembling only the mass contributions
    matrices.addObject('MatrixLinearSystem', template="CompressedRowSparseMatrixMat3x3d", name='system')
    mass_matrix = matrices.addObject('MatrixLinearSystem', template="CompressedRowSparseMatrixMat3x3d", name='M',
                                     assembleStiffness=False, assembleDamping=False, assembleGeometricStiffness=False,
                                     applyProjectiveConstraints=False)  # This system assembles only the mass contributions
    matrices.addObject(GlobalMatrixAccessController('MatrixAccessor', name='matrixAccessor', mass_matrix=mass_matrix))

    # This component distributes the matrix contributions to the other systems
    root.addObject('CompositeLinearSystem', template="CompressedRowSparseMatrixMat3x3d", name="solverSystem",
                   linearSystems="@matrices/system @matrices/M", solverLinearSystem="@matrices/system")
    # It is important to define the linear system in the linear solver: the CompositeLinearSystem
    root.addObject('SparseLDLSolver', ordering='Natural', template="CompressedRowSparseMatrixMat3x3d",
                   linearSystem="@solverSystem")

    beam1 = root.addChild('Beam1')
    create_beam(beam1, 1, 0)

    beam2 = root.addChild('Beam2')
    create_beam(beam2, 2, 10)

    return root


class LocalMatrixAccessController(Sofa.Core.Controller):

    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.mass = kwargs.get("mass")

    def onAnimateEndEvent(self, event):
        mass_matrix = self.mass.assembleMMatrix()

        print('====================================')
        print('Mass matrix', self.mass.getPathName())
        print('====================================')
        print('dtype: ' + str(mass_matrix.dtype))
        print('shape: ' + str(mass_matrix.shape))
        print('ndim: ' + str(mass_matrix.ndim))
        print('nnz: ' + str(mass_matrix.nnz))
        print('norm: ' + str(sparse.linalg.norm(mass_matrix)))

        if exportCSV:
            np.savetxt(self.mass.getName() + '.csv', mass_matrix.toarray(), delimiter=',')
        if showImage:
            plt.imshow(mass_matrix.toarray(), interpolation='nearest', cmap='gist_gray')
            plt.show(block=False)


class GlobalMatrixAccessController(Sofa.Core.Controller):

    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.mass_system = kwargs.get("mass_matrix")

    def onAnimateEndEvent(self, event):
        mass_matrix = self.mass_system.A()

        print('====================================')
        print('Mass matrix', self.mass_system.getPathName())
        print('====================================')
        print('dtype: ' + str(mass_matrix.dtype))
        print('shape: ' + str(mass_matrix.shape))
        print('ndim: ' + str(mass_matrix.ndim))
        print('nnz: ' + str(mass_matrix.nnz))
        print('norm: ' + str(sparse.linalg.norm(mass_matrix)))

        if exportCSV:
            np.savetxt(self.mass_system.getName() + '.csv', mass_matrix.toarray(), delimiter=',')
        if showImage:
            plt.imshow(mass_matrix.toarray(), interpolation='nearest', cmap='gist_gray')
            plt.show(block=False)

