# Required import for python
import Sofa
import SofaRuntime
from Sofa import SofaLinearSystem
from Sofa import SofaLinearSolver
from scipy import sparse

def createBeam(root, matrix_type):
    node = root.addChild(matrix_type)
    node.addObject('EulerImplicitSolver', rayleighStiffness="0.1", rayleighMass="0.1")
    linear_system = node.addObject('MatrixLinearSystem', template=matrix_type)

    node.addObject('MechanicalObject', name="DoFs")
    node.addObject('UniformMass', name="mass", totalMass="320")
    node.addObject('RegularGridTopology', name="grid", nx="4", ny="4", nz="20", xmin="-9", xmax="-6", ymin="0", ymax="3", zmin="0", zmax="19")
    node.addObject('BoxROI', name="box", box="-10 -1 -0.0001  -5 4 0.0001")
    node.addObject('FixedProjectiveConstraint', indices="@box.indices")
    node.addObject('HexahedronFEMForceField', name="FEM", youngModulus="4000", poissonRatio="0.3", method="large")

    node.addObject(MatrixAccessController('MatrixAccessor', name=f'matrixAccessor{matrix_type}', linear_system=linear_system))

    return node


# Function called when the scene graph is being created
def createScene(root):

    root.addObject('VisualStyle', displayFlags="showBehaviorModels showForceFields")

    root.addObject("RequiredPlugin", pluginName=['Sofa.Component.Mass',
                                                    'Sofa.Component.StateContainer',
                                                    'Sofa.Component.Topology.Container.Grid',
                                                    'Sofa.Component.Visual'
                                                    ])

    root.addObject('DefaultAnimationLoop', parallelODESolving=True)
    root.addObject('DefaultVisualManagerLoop')

    plugins = root.addChild('plugins')
    plugins.addObject('RequiredPlugin', name='Sofa.Component.Constraint.Projective')
    plugins.addObject('RequiredPlugin', name='Sofa.Component.Engine.Select')
    plugins.addObject('RequiredPlugin', name='Sofa.Component.LinearSolver.Direct')
    plugins.addObject('RequiredPlugin', name='Sofa.Component.LinearSystem')
    plugins.addObject('RequiredPlugin', name='Sofa.Component.ODESolver.Backward')
    plugins.addObject('RequiredPlugin', name='Sofa.Component.SolidMechanics.FEM.Elastic')

    node_crs = createBeam(root, 'CompressedRowSparseMatrixMat3x3d')
    node_crs.addObject('SparseLDLSolver', template="CompressedRowSparseMatrixMat3x3d")

    node_full = createBeam(root, 'FullMatrix')
    node_full.addObject('CGLinearSolver', template="FullMatrix")

    return root


class MatrixAccessController(Sofa.Core.Controller):


    def __init__(self, *args, **kwargs):
        print('Initialize controller')
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.linear_system = kwargs.get("linear_system")
        print(f'Type linear system: {type(self.linear_system)}')

    def onAnimateEndEvent(self, event):
        system_matrix = self.linear_system.get_system_matrix()
        rhs = self.linear_system.get_rhs_vector()
        solution = self.linear_system.get_solution_vector()

        print('====================================')
        print(f'Global system matrix {self.getName()}')
        print('====================================')
        print('dtype: ' + str(system_matrix.dtype))
        print('shape: ' + str(system_matrix.shape))
        print('ndim: ' + str(system_matrix.ndim))
        if hasattr(system_matrix, 'nnz'): #if the matrix is sparse
            print('nnz: ' + str(system_matrix.nnz))

        print('====================================')
        print(f'System right hand side {self.getName()}')
        print('====================================')
        print('dtype: ' + str(rhs.dtype))
        print('shape: ' + str(rhs.shape))
        print('ndim: ' + str(rhs.ndim))

        print('====================================')
        print(f'System solution {self.getName()}')
        print('====================================')
        print('dtype: ' + str(solution.dtype))
        print('shape: ' + str(solution.shape))
        print('ndim: ' + str(solution.ndim))

