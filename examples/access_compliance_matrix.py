﻿# Required import for python
import Sofa
import SofaRuntime
from Sofa import SofaConstraintSolver
import numpy as np


# Function called when the scene graph is being created
def createScene(root):
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
    constraint_solver = root.addObject("GenericConstraintSolver", tolerance=1e-9, maxIterations=1000)
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

    root.addObject(MatrixAccessController('MatrixAccessor', name='matrixAccessor', constraint_solver=constraint_solver))


class MatrixAccessController(Sofa.Core.Controller):

    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.constraint_solver = kwargs.get("constraint_solver")

    def onAnimateEndEvent(self, event):
        compliance_matrix = self.constraint_solver.W()

        print('====================================')
        print('Compliance matrix in constraint space')
        print('====================================')
        print('dtype: ' + str(compliance_matrix.dtype))
        print('shape: ' + str(compliance_matrix.shape))
        print('ndim: ' + str(compliance_matrix.ndim))

        with np.printoptions(precision=3, suppress=False, threshold=np.inf):
            print(str(compliance_matrix))

        print('====================================')
        print('Lambda force')
        print('====================================')

        lambda_vector = self.constraint_solver.lambda_force()
        print(str(lambda_vector))
