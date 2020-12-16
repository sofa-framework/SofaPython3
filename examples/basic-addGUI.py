# Required import for python
import Sofa
import SofaRuntime
import Sofa.Gui


USE_GUI = True


def main():
        # Make sure to load all SOFA libraries
        SofaRuntime.importPlugin("SofaOpenglVisual")

        # Call the above function to create the scene graph
        rootNode = createScene(Sofa.Core.Node("root"))

        if not USE_GUI:
                Sofa.Simulation.init(rootNode)
                for iteration in range(10):
                        Sofa.Simulation.animate(rootNode, rootNode.dt.value)
        else:
                # Find out the supported GUIs
                print ("Supported GUIs are: " + Sofa.Gui.GUIManager.ListSupportedGUI(","))
                # Launch the GUI
                Sofa.Gui.GUIManager.Init("myscene", "qglviewer")
                Sofa.Gui.GUIManager.createGUI(rootNode, __file__)
                Sofa.Gui.GUIManager.SetDimension(1080, 1080)
                # Initialization of the scene will be done here
                Sofa.Gui.GUIManager.MainLoop(rootNode)
                Sofa.Gui.GUIManager.closeGUI()
                print("GUI was closed")

        print("Simulation is done.")


# Function called when the scene graph is being created
def createScene(root):
        # Scene must now include a VisualLoop
        root.addObject('DefaultVisualManagerLoop')

        # Scene must now include a AnimationLoop
        root.addObject('DefaultAnimationLoop')

        # Add new nodes and objects in the scene
        node1 = root.addChild("Node1")
        node2 = root.addChild("Node2")
        
        node1.addObject("MechanicalObject")
        node1.addObject("OglModel")
        
        node2.addObject("MechanicalObject")
        node2.addObject("OglModel")

        return root


# Function used only if this script is called from a python environment
if __name__ == '__main__':
    main()