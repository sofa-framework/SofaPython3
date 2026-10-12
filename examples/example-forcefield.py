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

    def addForce(self, m, out_force, pos, vel):
        with out_force.writeableArray() as wa:
            wa[:] += ( (self.initpos-pos.value) * self.ks.value )

    def addDForce(self, params, df, dx):
        kFactor = params['kFactor']
        with df.writeableArray() as dfwa:
            dfwa[:] -= dx.value * self.ks.value * kFactor

    def addKToMatrix(self, params, number_of_nodes, number_of_dofs_per_node):
        K = []
        kFactor = params['kFactor']
        for n in range(0, number_of_nodes):
            for d in range(0, number_of_dofs_per_node):
                K.append([number_of_dofs_per_node * n + d, number_of_dofs_per_node * n + d, -kFactor * self.kd.value])
        return np.asarray(K)

    def buildStiffnessMatrix(self, number_of_nodes, number_of_dofs_per_node):
        K = []
        for n in range(0, number_of_nodes):
            for d in range(0, number_of_dofs_per_node):
                K.append([number_of_dofs_per_node * n + d, number_of_dofs_per_node * n + d, -self.ks.value])
        return np.asarray(K)


def createScene(root):

        root.addObject("RequiredPlugin", pluginName=["Sofa.GL.Component",
                                                     "Sofa.Component.IntegrationScheme.Backward",
                                                     "Sofa.Component.LinearSolver.Direct",
                                                     "Sofa.Component.LinearSolver.Iterative",
                                                     "Sofa.Component.Mass",
                                                     "Sofa.Component.StateContainer",
                                                     "Sofa.Component.Visual"
                                                      ])

        root.addObject("LineAxis")
        root.addObject("DefaultAnimationLoop", name="loop")

        def add_spring_simulation(node, start_position, color):
            node.addObject("EulerImplicitIntegrationScheme", rayleighStiffness=0.1, rayleighMass=0.1)

            o = node.addChild("Object")
            c = o.addObject("MechanicalObject", name="mechanical", position=[start_position + 0.0,0.0,0.0, start_position + 1.0,0.0,0.0], showColor=color)
            c.showObject = True
            c.drawMode = 1

            o.addObject("UniformMass", name="mass", totalMass=[0.1])
            o.addObject( RestShapeForceField(name="spring", ks=2.0, kd=0.1))

        with root.addChild("Direct") as direct:
            add_spring_simulation(direct, 0.0, [0,1,0,1])
            direct.addObject("SparseLDLSolver", template="CompressedRowSparseMatrixd")
        with root.addChild("Iterative") as iterative:
            add_spring_simulation(iterative, 0.2, [1,0,0,1])
            iterative.addObject("CGLinearSolver", tolerance=1e-12, threshold=1e-12, iterations=25)

        return root


def main():
    import SofaImGui
    import Sofa.Gui

    root=Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.initRoot(root)

    Sofa.Gui.GUIManager.Init("myscene", "imgui")
    Sofa.Gui.GUIManager.createGUI(root, __file__)
    Sofa.Gui.GUIManager.SetDimension(1080, 1080)
    Sofa.Gui.GUIManager.MainLoop(root)
    Sofa.Gui.GUIManager.closeGUI()

    print("End of simulation.")


if __name__ == '__main__':
    main()
