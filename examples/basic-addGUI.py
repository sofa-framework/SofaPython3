# Choose in your script to activate or not the GUI
USE_GUI = True

def main():
    # Required import for python
    import Sofa
    import SofaImGui

    #Create the root node
    root = Sofa.Core.Node("root")
    # Call the below 'createScene' function to create the scene graph
    createScene(root)
    Sofa.Simulation.initRoot(root)

    if not USE_GUI:
        for iteration in range(10):
            Sofa.Simulation.animate(root, root.dt.value)
    else:
        # Required import to manage the GUI
        import Sofa.Gui

        # Find out the supported GUIs
        print ("Supported GUIs are: " + Sofa.Gui.GUIManager.ListSupportedGUI(","))
        # Launch the GUI (imgui is now by default, to use Qt please refer to the example "basic-useQtGui.py")
        Sofa.Gui.GUIManager.Init("myscene", "imgui")
        Sofa.Gui.GUIManager.createGUI(root, __file__)
        Sofa.Gui.GUIManager.SetDimension(1080, 1080)
        # Initialization of the scene will be done here
        Sofa.Gui.GUIManager.MainLoop(root)
        Sofa.Gui.GUIManager.closeGUI()
        print("GUI was closed")

    print("Simulation is done.")


# Function called when the scene graph is being created
def createScene(root):

    root.addObject('RequiredPlugin', name='Sofa.Component.StateContainer')

    # Scene must now include a AnimationLoop
    root.addObject('DefaultAnimationLoop')

    # Add new nodes and objects in the scene
    node1 = root.addChild("Node1")
    node2 = root.addChild("Node2")

    node1.addObject("MechanicalObject", template="Rigid3d", position="0 0 0   0 0 0 1", showObject="1")

    node2.addObject("MechanicalObject", template="Rigid3d", position="1 1 1   0 0 0 1", showObject="1")

    return root


# Function used only if this script is called from a python environment
if __name__ == '__main__':
    main()
