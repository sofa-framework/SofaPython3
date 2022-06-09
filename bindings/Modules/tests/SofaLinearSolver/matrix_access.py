import unittest
import Sofa.Core
import Sofa.Components
from Sofa import SofaLinearSolver

class Test(unittest.TestCase):

    def simulate_beam(self, linear_solver_template):
        root = Sofa.Core.Node("rootNode")

        loop = root.addObject('DefaultAnimationLoop')

        root.addObject('RequiredPlugin', name='Sofa.Component.ODESolver.Backward')
        root.addObject('RequiredPlugin', name='Sofa.Component.LinearSolver.Direct')
        root.addObject('RequiredPlugin', name='Sofa.Component.Engine.Select')
        root.addObject('RequiredPlugin', name='Sofa.Component.Constraint.Projective')
        root.addObject('RequiredPlugin', name='Sofa.Component.SolidMechanics.FEM.Elastic')

        root.addObject('EulerImplicitSolver', rayleighStiffness="0.1", rayleighMass="0.1")
        linear_solver = root.addObject('SparseLDLSolver', applyPermutation="false", template=linear_solver_template)

        root.addObject('MechanicalObject', name="DoFs")
        root.addObject('UniformMass', name="mass", totalMass="320")
        root.addObject('RegularGridTopology', name="grid", nx="4", ny="4", nz="20", xmin="-9", xmax="-6", ymin="0", ymax="3", zmin="0", zmax="19")
        root.addObject('BoxROI', name="box", box="-10 -1 -0.0001  -5 4 0.0001")
        root.addObject('FixedConstraint', indices="@box.indices")
        root.addObject('HexahedronFEMForceField', name="FEM", youngModulus="4000", poissonRatio="0.3", method="large")

        matrix_accessor = root.addObject(MatrixAccessController('MatrixAccessor', name='matrixAccessor', linear_solver=linear_solver))

        Sofa.Simulation.init(root)
        Sofa.Simulation.animate(root, 0.0001)

        return matrix_accessor.system_matrix

    def test_matrix_access_blocks_scalar(self):
        A = self.simulate_beam("CompressedRowSparseMatrixd")

        self.assertEqual(A.ndim, 2)
        self.assertEqual(A.shape, (960, 960))
        self.assertEqual(A.nnz, 52200)

    def test_matrix_access_blocks3x3(self):
        A = self.simulate_beam("CompressedRowSparseMatrixMat3x3d")

        self.assertEqual(A.ndim, 2)
        self.assertEqual(A.shape, (960, 960))
        self.assertEqual(A.nnz, 41629)


class MatrixAccessController(Sofa.Core.Controller):

    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.linear_solver = kwargs.get("linear_solver")

    def onAnimateEndEvent(self, event):
        self.system_matrix = self.linear_solver.A()
        self.rhs = self.linear_solver.b()
        self.solution = self.linear_solver.x()
