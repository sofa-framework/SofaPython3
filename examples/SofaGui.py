import Sofa
import Sofa.Core
import Sofa.Simulation
import Sofa.Gui

print ("Supported GUIs are " + Sofa.Gui.GUIManager.ListSupportedGUI(","))


root = Sofa.Core.Node("root")
# Create the rest of the scene here...


# Initialize all components found in the scene
Sofa.Simulation.init(root)


# Launch the GUI
Sofa.Gui.GUIManager.Init("simple_scene", "qt")
Sofa.Gui.GUIManager.createGUI(root)
Sofa.Gui.GUIManager.MainLoop(root)
Sofa.Gui.GUIManager.closeGUI()
