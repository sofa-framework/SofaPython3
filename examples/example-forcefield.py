"""Implementation of a RestShapeForceField in python"""
# coding: utf8
import Sofa
import SofaRuntime
import Sofa.Gui
import numpy as np

class RestShapeForceField(Sofa.Core.ForceField):
    """Implementation of a RestShapeForceField in python"""
    def __init__(self, ks=1.0, kd=1.0, *args, **kwargs):
        Sofa.Core.ForceField.__init__(self, *args, **kwargs)
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
        #print(" Python::addDForce: ", params)
        
    #def addKToMatrix(self, a, b):
    #    print(" Python::addKToMatrix: ", a, " ", b)

def createScene(node):
        node.addObject("RequiredPlugin", name="SofaOpenglVisual")
        node.addObject("RequiredPlugin", name="SofaSparseSolver")
        node.addObject("OglLineAxis")
        node.addObject("DefaultAnimationLoop", name="loop")
        node.addObject("EulerImplicit")
        node.addObject("CGLinearSolver", tolerance=1e-12, threshold=1e-12)

        o = node.addChild("Object")
        c = o.addObject("MechanicalObject", name="mechanical", position=[0.0,0.0,0.0, 1.0,0.0,0.0])
        c.showObject = True
        c.showColor = [1.0,0.0,0.0,1.0]
        c.drawMode = 1

        o.addObject("UniformMass", name="mass", totalMass=[0.1])
        o.addObject( RestShapeForceField(name="CPPObject", ks=2.0, kd=0.1))

        return node

def main():
    SofaRuntime.importPlugin("SofaOpenglVisual")
    SofaRuntime.importPlugin("SofaBaseMechanics")
    SofaRuntime.importPlugin("SofaImplicitOdeSolver")

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
