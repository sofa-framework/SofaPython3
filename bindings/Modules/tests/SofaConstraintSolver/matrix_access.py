import unittest
import Sofa.Core
import Sofa.Components
from Sofa import SofaConstraintSolver

class Test(unittest.TestCase):

    def simulate_pendulum(self):
        root = Sofa.Core.Node("rootNode")

        root.addObject("RequiredPlugin", pluginName=["Sofa.Component.AnimationLoop",
                                                     "Sofa.Component.Constraint.Lagrangian.Correction",
                                                     "Sofa.Component.Constraint.Lagrangian.Model",
                                                     "Sofa.Component.Constraint.Lagrangian.Solver",
                                                     "Sofa.Component.Constraint.Projective",
                                                     "Sofa.Component.IO.Mesh",
                                                     "Sofa.Component.LinearSolver.Direct",
                                                     "Sofa.Component.Mapping.MappedMatrix",
                                                     "Sofa.Component.Mass",
                                                     "Sofa.Component.ODESolver.Backward",
                                                     "Sofa.Component.Topology.Container.Dynamic"])

        root.addObject("FreeMotionAnimationLoop", solveVelocityConstraintFirst=True)
        root.addObject("GenericConstraintSolver", name="constraint_solver", tolerance=1e-9, maxIterations=1000)
        root.addObject("StringMeshCreator", name="loader", resolution="20")

        root.addObject("EulerImplicitSolver")
        root.addObject("SparseCholeskySolver")
        root.addObject("GenericConstraintCorrection")

        root.addObject("EdgeSetTopologyContainer", position="@loader.position", edges="@loader.edges")
        root.addObject("MechanicalObject", name="defoDOF", template="Vec3d")
        root.addObject("EdgeSetGeometryAlgorithms", drawEdges=True)
        root.addObject("FixedConstraint", indices=[0])
        root.addObject("DiagonalMass", name="mass", totalMass="1e-3")
        root.addObject("MappingGeometricStiffnessForceField", mapping="@./extensionsNode/distanceMapping")

        ext = root.addChild("extensionsNode")
        ext.addObject("MechanicalObject", template="Vec1d", name="extensionsDOF")
        ext.addObject("DistanceMapping", name="distanceMapping")
        ext.addObject("UniformConstraint", template="Vec1d", iterative=True)

        Sofa.Simulation.init(root)
        Sofa.Simulation.animate(root, 0.0001)

        return root

    def test_matrix_access(self):
        root = self.simulate_pendulum()
        W = root.constraint_solver.W()

        self.assertEqual(W.ndim, 2)
        self.assertEqual(W.shape, (38, 38))
