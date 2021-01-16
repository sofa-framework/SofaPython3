import Sofa
import Sofa.Gui
import SofaRuntime
import tempfile
import os



def main():
	# Make sure to load all SOFA libraries
	SofaRuntime.importPlugin("SofaOpenglVisual")
	
	root = Sofa.Core.Node("root")

	# Call the above function to create the scene graph
	scene="""
			<Node name="rootNode" dt="0.005" gravity="0 0 0">
			
				<DefaultVisualManagerLoop/>
				<DefaultAnimationLoop/>

				<Node name="child1">
					<MechanicalObject template="Rigid3d" position="0 0 0   0 0 0 1" showObject="1"/>
				</Node>

				<Node name="child2">
					<MechanicalObject template="Rigid3d" position="1 1 1   0 0 0 1" showObject="1"/>
				</Node>
			</Node>
		"""
	tf = tempfile.NamedTemporaryFile(mode="w+t", suffix=".scn", delete=False)
	tf.write(scene)
	tf.flush()
	node = Sofa.Simulation.load(tf.name)

	# Find out the supported GUIs
	print ("Supported GUIs are: " + Sofa.Gui.GUIManager.ListSupportedGUI(","))
	# Launch the GUI (qt or qglviewer)
	Sofa.Gui.GUIManager.Init("myscene", "qglviewer")
	Sofa.Gui.GUIManager.createGUI(node, __file__)
	Sofa.Gui.GUIManager.SetDimension(1080, 1080)
	# Initialization of the scene will be done here
	Sofa.Gui.GUIManager.MainLoop(node)
	Sofa.Gui.GUIManager.closeGUI()
	print("GUI was closed")
	print("Simulation is done.")
	os.remove(tf.name)

# Function used only if this script is called from a python environment
if __name__ == '__main__':
    main()