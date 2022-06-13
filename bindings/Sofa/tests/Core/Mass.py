# coding: utf8

import unittest
import Sofa
import Sofa.Core
import Sofa.Helper
import Sofa.Simulation
import SofaRuntime

class Test(unittest.TestCase):

    @staticmethod
    def simulate_beam(linear_solver_template):
        root = Sofa.Core.Node("rootNode")

        root.addObject('DefaultAnimationLoop')

        root.addObject('RequiredPlugin', name='Sofa.Component.ODESolver.Backward')
        root.addObject('RequiredPlugin', name='Sofa.Component.LinearSolver.Direct')
        root.addObject('RequiredPlugin', name='Sofa.Component.Engine.Select')
        root.addObject('RequiredPlugin', name='Sofa.Component.Constraint.Projective')
        root.addObject('RequiredPlugin', name='Sofa.Component.SolidMechanics.FEM.Elastic')
        root.addObject('RequiredPlugin', name='Sofa.Component.Mass')

        root.addObject('EulerImplicitSolver', rayleighStiffness="0.1", rayleighMass="0.1")
        root.addObject('SparseLDLSolver', applyPermutation="false", template=linear_solver_template)

        root.addObject('MechanicalObject', name="DoFs")
        root.addObject('MeshMatrixMass', name="mass", totalMass="320")
        root.addObject('RegularGridTopology', name="grid", nx="4", ny="4", nz="20", xmin="-9", xmax="-6", ymin="0", ymax="3", zmin="0", zmax="19")
        root.addObject('BoxROI', name="box", box=[-10, -1, -0.0001,  -5, 4, 0.0001])
        root.addObject('FixedConstraint', indices="@box.indices")
        root.addObject('HexahedronFEMForceField', name="FEM", youngModulus="4000", poissonRatio="0.3", method="large")

        Sofa.Simulation.init(root)
        Sofa.Simulation.animate(root, 0.0001)

        return root

    def test_mass_matrix_access_scalar(self):

        root = self.simulate_beam("CompressedRowSparseMatrixd")
        M = root.mass.assembleMMatrix()

        self.assertEqual(M.ndim, 2)
        self.assertEqual(M.shape, (960, 960))
        self.assertEqual(M.nnz, 9480)

    def test_mass_matrix_access_blocks3x3(self):

        root = self.simulate_beam("CompressedRowSparseMatrixMat3x3d")
        M = root.mass.assembleMMatrix()

        self.assertEqual(M.ndim, 2)
        self.assertEqual(M.shape, (960, 960))
        self.assertEqual(M.nnz, 9480)
