"""Implementation of an identity mapping in python"""
# coding: utf8
import Sofa
import numpy as np

class IdentityMapping(Sofa.Core.Mapping_Vec3d_Vec3d):
    """Implementation of a Mapping in python"""
    def __init__(self, *args, **kwargs):

        ## This is just an ugly trick to transform pointer to object into sofa linkpath. 
        for k,v in kwargs.items():
            if k == "input":
                kwargs[k] = v.linkpath
            elif k == "output":
                kwargs[k] = v.linkpath

        Sofa.Core.Mapping_Vec3d_Vec3d.__init__(self, *args, **kwargs)

        # let's compute the positions initial delta.
        input_position = kwargs["input"].target().position.value.copy()
        output_position = kwargs["output"].target().position.value.copy()
        self.delta = output_position - input_position
        
    def apply(self, m, outCoord, inCoord):
        print("PYTHON(🐍) APPLY ", outCoord, inCoord)
        with outCoord.writeableArray() as wa:
            wa[:] = inCoord.value + self.delta

    def applyJ(self, m, outDeriv, inDeriv):
        print("PYTHON(🐍) APPLY-J", outDeriv, inDeriv)

    def applyJT(self, m, outDeriv, inDeriv):
        print("PYTHON(🐍) APPLY-JT", outDeriv, inDeriv)

    def applyConstrainsJT(self, m, outDeriv, inDeriv):
        print("PYTHON(🐍) APPLY-JT for constraints, data are [⋱]", m, outDeriv, inDeriv)
        print("Constraints ", inDeriv.value)




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

        root.addChild("Modelling")

        ######### OBJECT #####################
        o = root.Modelling.addChild("Object")
        c = o.addObject("MechanicalObject", name="mechanical", position=[0.0,0.0,0.0, 1.0,0.0,0.0])
        c.showObject = True
        c.showColor = [1.0,0.0,0.0,1.0]
        c.drawMode = 1
        o.addObject("UniformMass", name="mass", totalMass=1.0)
        
        ######### OBJECT #####################
        m = o.addChild("MappedDof")
        sm = m.addObject("MechanicalObject", name="mapped_quantities", position=[0.0,0.5,0.0, 1.0, 0.5, 0.0])
        sm.showObject = True
        sm.showColor = [1.0,0.0,1.0,0.7]
        sm.drawMode = 1
        m.addObject( IdentityMapping(name="CPPObject", input=c, output=sm ) )
        
        root.addChild("Simulation")
        root.Simulation.addObject("EulerImplicitSolver")
        root.Simulation.addObject("CGLinearSolver", tolerance=1e-12, threshold=1e-12, iterations=25)
        root.Simulation.addChild(root.Modelling)

        return root


def main():
    import SofaRuntime
    import Sofa.Gui
    import SofaQt

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
