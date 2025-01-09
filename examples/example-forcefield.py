"""Implementation of a RestShapeForceField in python"""
# coding: utf8
import Sofa
import numpy as np


class RestShapeForceField(Sofa.Core.ForceFieldVec3d):
    """Implementation of a RestShapeForceField in python"""
    def __init__(self, ks=1.0, kd=1.0, *args, **kwargs):
        Sofa.Core.ForceFieldVec3d.__init__(self, *args, **kwargs)
        self.addData("ks", type="float", value=ks, help="The stiffness spring", group="Spring's Properties")                  
        self.addData("kd", type="float", value=kd, help="The damping spring", group="Spring's Properties")                  
        
    def init(self):
        mstate = self.getContext().mechanical
        self.initpos = mstate.position.array().copy()
        self.k = np.zeros((1,1))
        self.f = []
        self.d = 0.5

    def addForce(self, m, out_force, pos, vel):
        with out_force.writeableArray() as wa:
            wa[:] += ( (self.initpos-pos.value) * self.ks.value  )
                 
    def addDForce(self, df, dx, params):
        pass
        
    #def addKToMatrix(self, a, b):
    #    print(" Python::addKToMatrix: ", a, " ", b)


def createScene(root):

        root.addObject("RequiredPlugin", pluginName=["Sofa.GL.Component",
                                                     "Sofa.Component.ODESolver.Backward",
                                                     "Sofa.Component.LinearSolver.Direct",
                                                     "Sofa.Component.LinearSolver.Iterative",
                                                     "Sofa.Component.Mass",
                                                     "Sofa.Component.StateContainer",
                                                     "Sofa.Component.Visual"
                                                      ])

        root.addObject("LineAxis")
        root.addObject("DefaultAnimationLoop", name="loop")
        root.addObject("EulerImplicitSolver")
        root.addObject("CGLinearSolver", tolerance=1e-12, threshold=1e-12, iterations=25)

        o = root.addChild("Object")
        c = o.addObject("MechanicalObject", name="mechanical", position=[0.0,0.0,0.0, 1.0,0.0,0.0])
        c.showObject = True
        c.showColor = [1.0,0.0,0.0,1.0]
        c.drawMode = 1

        o.addObject("UniformMass", name="mass", totalMass=[0.1])
        o.addObject( RestShapeForceField(name="CPPObject", ks=2.0, kd=0.1))

        return root


def main():
    import SofaRuntime
    import Sofa.Gui

    root=Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.initRoot(root)

    Sofa.Gui.GUIManager.Init("myscene", "qglviewer")
    Sofa.Gui.GUIManager.createGUI(root, __file__)
    Sofa.Gui.GUIManager.SetDimension(1080, 1080)
    Sofa.Gui.GUIManager.MainLoop(root)
    Sofa.Gui.GUIManager.closeGUI()

    print("End of simulation.")


if __name__ == '__main__':
    main()
