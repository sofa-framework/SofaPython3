import Sofa
import Sofa.Gui
from Sofa.Helper import msg_info
import numpy as np


# This python script shows the functions to be implemented
# in order to create your ForceField in python
class EmptyForceField(Sofa.Core.ForceField):
    def __init__(self, *args, **kwargs):
        Sofa.Core.ForceField.__init__(self, *args, **kwargs)
        pass

    # Function called at the component initialization
    def init(self):
        msg_info('init: not implemented yet')
        pass

    # Function implementing the explicit force f(x(t), v(t))
    def addForce(self, m, forces, pos, vel):
        msg_info('addForce: not implemented yet')
        pass

    # Function implementing the matrix-vector multiplication
    # between the derivative of the force f with regards to
    # the degrees of freedom, multiplied by a vector dx
    def addDForce(self, m, dforce, dx):
        msg_info('addDForce: not implemented yet')
        pass

    # Function implementing the matrix corresponding to the
    # derivative of the force f with regards to the degrees
    # of freedom
    def addKToMatrix(self, mparams, nNodes, nDofs):
        msg_info('addKToMatrix: not implemented yet')
        pass


def createScene(root):
    root.dt = 0.01
    root.addObject('DefaultVisualManagerLoop')
    root.addObject('DefaultAnimationLoop')

    node1 = root.addChild("Node1")

    node1.addObject('EulerImplicitSolver')
    node1.addObject('CGLinearSolver', iterations="100", tolerance="10-3", threshold="10-3")
    node1.addObject('MechanicalObject', template="Vec3d")

    # Add our python forcefield in the scene
    node1.addObject( EmptyForceField(name="MyEmptyForceField") )


def main():
    root=Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.init(root)

    Sofa.Gui.GUIManager.Init("myscene", "qglviewer")
    Sofa.Gui.GUIManager.createGUI(root, __file__)
    Sofa.Gui.GUIManager.SetDimension(1080, 1080)
    Sofa.Gui.GUIManager.MainLoop(root)
    Sofa.Gui.GUIManager.closeGUI()

    print("End of simulation.")


if __name__ == '__main__':
    main()
