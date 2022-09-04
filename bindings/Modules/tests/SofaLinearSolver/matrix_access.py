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
        root.addObject('RequiredPlugin', name='Sofa.Component.Mass')

        root.addObject('EulerImplicitSolver', rayleighStiffness="0.1", rayleighMass="0.1")
        linear_solver = root.addObject('SparseLDLSolver', name='linear_solver', applyPermutation="false", template=linear_solver_template)

        root.addObject('MechanicalObject', name="DoFs")
        root.addObject('UniformMass', name="mass", totalMass="320")
        root.addObject('RegularGridTopology', name="grid", nx="4", ny="4", nz="20", xmin="-9", xmax="-6", ymin="0", ymax="3", zmin="0", zmax="19")
        root.addObject('BoxROI', name="box", box=[-10, -1, -0.0001, -5, 4, 0.0001])
        root.addObject('FixedConstraint', indices="@box.indices")
        root.addObject('HexahedronFEMForceField', name="FEM", youngModulus="4000", poissonRatio="0.3", method="large")

        Sofa.Simulation.init(root)
        Sofa.Simulation.animate(root, 0.0001)

        return root

    def test_matrix_access_blocks_scalar(self):
        root = self.simulate_beam("CompressedRowSparseMatrixd")
        A = root.linear_solver.A()

        self.assertEqual(A.ndim, 2)
        self.assertEqual(A.shape, (960, 960))
        self.assertGreater(A.nnz, 960) # Should be Equal to one specific value but a different between OS has been reported here: https://github.com/sofa-framework/sofa/issues/3036

    def test_matrix_access_blocks3x3(self):
        root = self.simulate_beam("CompressedRowSparseMatrixMat3x3d")
        A = root.linear_solver.A()

        self.assertEqual(A.ndim, 2)
        self.assertEqual(A.shape, (960, 960))
        self.assertGreater(A.nnz, 960) # Should be Equal to one specific value but a different between OS has been reported here: https://github.com/sofa-framework/sofa/issues/3036
