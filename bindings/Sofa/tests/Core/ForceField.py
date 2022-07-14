# coding: utf8

import unittest
import Sofa
import Sofa.Core
import Sofa.Helper
import Sofa.Simulation
import SofaRuntime
from MyRestShapeForceField import *
from numpy.linalg import norm as np_norm


def createIntegrationScheme(node, use_implicit_scheme):
    if use_implicit_scheme is True:
        node.addObject('EulerImplicitSolver', name='odeImplicitSolver',
                       rayleighStiffness='0.1', rayleighMass='0.1')
    else:
        node.addObject('EulerExplicitSolver', name='odeExplicitSolver')


def createSolver(node, use_iterative_solver):
    if use_iterative_solver == True:
        node.addObject('CGLinearSolver', name='linearSolver',
                       iterations=30, tolerance=1.e-9, threshold=1.e-9)
    else:
        node.addObject('SparseCholeskySolver', name='ldlSolver')


def createParticle(node, node_name, use_implicit_scheme, use_iterative_solver):
    p = node.addChild(node_name)
    createIntegrationScheme(p, use_implicit_scheme)
    createSolver(p, use_iterative_solver)
    dofs = p.addObject('MechanicalObject', name="MO", position=[0, 0, 0])
    p.addObject('UniformMass', totalMass=1.0)

    myRSSFF = NaiveRestShapeSpringsForcefield(name="Springs",
                                           stiffness=50,
                                           mstate=dofs, rest_pos=dofs.rest_position)
    p.addObject(myRSSFF)


def rssffScene(use_implicit_scheme=True, use_iterative_solver=True):
    node = Sofa.Core.Node("root")
    node.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
    node.addObject("RequiredPlugin", name="Sofa.Component.LinearSolver")
    node.addObject("RequiredPlugin", name="Sofa.Component.ODESolver")
    node.addObject("RequiredPlugin", name="Sofa.Component.Mass")

    node.gravity = [0, -10, 0]
    createParticle(node, "particle", use_implicit_scheme, use_iterative_solver)
    return node


class Test(unittest.TestCase):
    def test_0_explicit(self):
        root = rssffScene(use_implicit_scheme=False,
                          use_iterative_solver=False)

        # do some steps here
        Sofa.Simulation.init(root)

        for i in range(0, 100):
            Sofa.Simulation.animate(root, root.dt.value)
            self.assertLess(np_norm(
                root.particle.MO.rest_position.value - root.particle.MO.position.value), 0.41,
                "Passed threshold on step " + str(i) + ".")
        return

    def test_1_implicit_iterative(self):
        root = rssffScene(use_implicit_scheme=True, use_iterative_solver=True)

        # do some steps here
        Sofa.Simulation.init(root)

        for i in range(0, 100):
            Sofa.Simulation.animate(root, root.dt.value)
            self.assertLess(np_norm(
                root.particle.MO.rest_position.value - root.particle.MO.position.value), 0.26,
                "Passed threshold on step " + str(i) + ".")
        return

    def test_2_implicit_direct(self):
        root = rssffScene(use_implicit_scheme=True, use_iterative_solver=False)

        # do some steps here
        Sofa.Simulation.init(root)

        for i in range(0, 100):
            Sofa.Simulation.animate(root, root.dt.value)
            self.assertLess(np_norm(
                root.particle.MO.rest_position.value - root.particle.MO.position.value), 0.26,
                "Passed threshold on step " + str(i) + ".")
        return

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
        root.addObject('UniformMass', name="mass", totalMass="320")
        root.addObject('RegularGridTopology', name="grid", nx="4", ny="4", nz="20", xmin="-9", xmax="-6", ymin="0", ymax="3", zmin="0", zmax="19")
        root.addObject('BoxROI', name="box", box=[-10, -1, -0.0001,  -5, 4, 0.0001])
        root.addObject('FixedConstraint', indices="@box.indices")
        root.addObject('HexahedronFEMForceField', name="FEM", youngModulus="4000", poissonRatio="0.3", method="large")

        Sofa.Simulation.init(root)
        Sofa.Simulation.animate(root, 0.0001)

        return root

    def test_stiffness_matrix_access_scalar(self):

        root = self.simulate_beam("CompressedRowSparseMatrixd")
        K = root.FEM.assembleKMatrix()

        self.assertEqual(K.ndim, 2)
        self.assertEqual(K.shape, (960, 960))
        self.assertEqual(K.nnz, 52200)

    def test_stiffness_matrix_access_blocks3x3(self):

        root = self.simulate_beam("CompressedRowSparseMatrixMat3x3d")
        K = root.FEM.assembleKMatrix()

        self.assertEqual(K.ndim, 2)
        self.assertEqual(K.shape, (960, 960))
        self.assertEqual(K.nnz, 52200)
