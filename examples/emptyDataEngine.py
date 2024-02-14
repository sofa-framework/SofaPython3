import Sofa
from Sofa.Helper import msg_info


# This python script shows the functions to be implemented
# in order to create your DataEngine in python
class EmptyDataEngine(Sofa.Core.DataEngine):

    def __init__(self, *args, **kwargs):
        Sofa.Core.DataEngine.__init__(self, *args, **kwargs)
        pass

    def init(self):
        pass

    def update():
        # Function called anytime an output is accessed while the component
        # is dirty (input has changed)
        msg_info('Not implemented yet')
        pass


def createScene(root):
    root.dt = 0.01
    root.bbox = [[-1, -1, -1],[1,1,1]]

    root.addObject('DefaultAnimationLoop')

    # Add our python forcefield in the scene
    root.addObject( EmptyDataEngine(name="MyEmptyDataEngine") )


def main():
    import Sofa.Gui
    import SofaRuntime

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
