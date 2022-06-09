# Required import for python
import Sofa
import SofaRuntime
from Sofa import SofaLinearSolver
from scipy import sparse

# Function called when the scene graph is being created
def createScene(root):

    root.addObject('VisualStyle', displayFlags="showBehaviorModels showForceFields")

    root.addObject('DefaultAnimationLoop')
    root.addObject('DefaultVisualManagerLoop')

    root.addObject('RequiredPlugin', name='Sofa.Component.ODESolver.Backward')
    root.addObject('RequiredPlugin', name='Sofa.Component.LinearSolver.Direct')
    root.addObject('RequiredPlugin', name='Sofa.Component.Engine.Select')
    root.addObject('RequiredPlugin', name='Sofa.Component.Constraint.Projective')
    root.addObject('RequiredPlugin', name='Sofa.Component.SolidMechanics.FEM.Elastic')

    root.addObject('EulerImplicitSolver', rayleighStiffness="0.1", rayleighMass="0.1")
    linear_solver = root.addObject('SparseLDLSolver', applyPermutation="false", template="CompressedRowSparseMatrixMat3x3d")

    root.addObject('MechanicalObject', name="DoFs")
    root.addObject('UniformMass', name="mass", totalMass="320")
    root.addObject('RegularGridTopology', name="grid", nx="4", ny="4", nz="20", xmin="-9", xmax="-6", ymin="0", ymax="3", zmin="0", zmax="19")
    root.addObject('BoxROI', name="box", box="-10 -1 -0.0001  -5 4 0.0001")
    root.addObject('FixedConstraint', indices="@box.indices")
    root.addObject('HexahedronFEMForceField', name="FEM", youngModulus="4000", poissonRatio="0.3", method="large")

    root.addObject(MatrixAccessController('MatrixAccessor', name='matrixAccessor', linear_solver=linear_solver))

    return root


class MatrixAccessController(Sofa.Core.Controller):


    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.linear_solver = kwargs.get("linear_solver")

    def onAnimateEndEvent(self, event):
        system_matrix = self.linear_solver.A()
        rhs = self.linear_solver.b()
        solution = self.linear_solver.x()

        print('====================================')
        print('Global system matrix')
        print('====================================')
        print('dtype: ' + str(system_matrix.dtype))
        print('shape: ' + str(system_matrix.shape))
        print('ndim: ' + str(system_matrix.ndim))
        print('nnz: ' + str(system_matrix.nnz))

        print('====================================')
        print('System right hand side')
        print('====================================')
        print('dtype: ' + str(rhs.dtype))
        print('shape: ' + str(rhs.shape))
        print('ndim: ' + str(rhs.ndim))

        print('====================================')
        print('System solution')
        print('====================================')
        print('dtype: ' + str(solution.dtype))
        print('shape: ' + str(solution.shape))
        print('ndim: ' + str(solution.ndim))

        pass

