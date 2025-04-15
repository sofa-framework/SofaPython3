from stlib.entities.rigid import Rigid
from stlib.entities.deformable import Deformable
from stlib.geometry.cube import CubeParameters
from stlib.geometry.file import FileParameters
from splib.simulation.headers import setupLagrangianCollision
from splib.simulation.linear_solvers import addLinearSolver
from splib.simulation.ode_solvers import addImplicitODE

#To be added in splib
def addSolvers(root):
    addLinearSolver(root)
    addImplicitODE(root)
    root.addObject("LinearSolverConstraintCorrection", linearsolver="@LinearSolver")

def createScene(root):
    root.gravity = [0, 0, -9.81]

    setupLagrangianCollision(root)
    addSolvers(root)

    rigidParams = Rigid.getParameters()
    rigidParams.geometry = CubeParameters([0, 0, 0], 1, 3)
    root.add(Rigid,rigidParams)

    deformableParams = Deformable.getParameters()
    #Add transformation somewhere here
    deformableParams.geometry = FileParameters("SofaScene/Logo.vtk")
    root.add(Deformable,deformableParams)

    return root
