# Required import for python
import Sofa
import SofaRuntime
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
                                                 "Sofa.Component.Topology.Container.Dynamic",
                                                 "Sofa.Component.Mapping.NonLinear",
                                                 "Sofa.Component.StateContainer"
                                                 ])

    root.addObject("FreeMotionAnimationLoop", solveVelocityConstraintFirst=True)
    root.addObject("GenericConstraintSolver", tolerance=1e-9, maxIterations=1000)
    root.addObject("StringMeshCreator", name="loader", resolution="20")

    root.addObject("EulerImplicitSolver")
    root.addObject("EigenSimplicialLLT", template='CompressedRowSparseMatrixMat3x3d')
    root.addObject("GenericConstraintCorrection")

    root.addObject("EdgeSetTopologyContainer", name="topo", position="@loader.position", edges="@loader.edges")
    mechanical_object = root.addObject("MechanicalObject", name="defoDOF", template="Vec3d")
    root.addObject("EdgeSetGeometryAlgorithms", drawEdges=True)
    root.addObject("FixedProjectiveConstraint", indices=[0])
    root.addObject("MeshMatrixMass", name="mass", totalMass="1e-3", lumping=True)

    ext = root.addChild("extensionsNode")
    ext.addObject("MechanicalObject", template="Vec1d", name="extensionsDOF")
    ext.addObject("DistanceMapping", name="distanceMapping", topology="@topo")
    ext.addObject("UniformLagrangianConstraint", template="Vec1d", iterative=True)

    root.addObject(MatrixAccessController('MatrixAccessor', name='matrixAccessor', mechanical_object=mechanical_object))


class MatrixAccessController(Sofa.Core.Controller):

    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.mechanical_object = kwargs.get("mechanical_object")

    def onBuildConstraintSystemEndEvent(self, event):
        constraint_matrix = self.mechanical_object.constraint.value

        print(self.mechanical_object.constraint.linkpath)

        print('====================================')
        print('Constraint matrix in constraint space')
        print('====================================')
        print('dtype: ' + str(constraint_matrix.dtype))
        print('shape: ' + str(constraint_matrix.shape))
        print('ndim: ' + str(constraint_matrix.ndim))

        print(constraint_matrix)


