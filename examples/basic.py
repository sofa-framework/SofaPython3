# Required import for python
import Sofa
import SofaRuntime


def main():
	# Make sure to load all SOFA libraries
	SofaRuntime.importPlugin("SofaBaseMechanics")

	# Call the above function to create the scene graph
	root = Sofa.Core.Node("root")
	createScene(root)

	# Once defined, initialization of the scene graph
	Sofa.Simulation.init(root)

	# Run the simulation for 10 steps
	for iteration in range(10):
		print(f'Iteration #{iteration}')
		Sofa.Simulation.animate(root, root.dt.value)

	print("Simulation made 10 time steps. Done")


# Function called when the scene graph is being created
def createScene(root):

	# Add new nodes and objects in the scene
	node1 = root.addChild("Node1")
	node2 = root.addChild("Node2")
	
	node1.addObject("MechanicalObject", template="Rigid3d", position="0 0 0   0 0 0 1", showObject="1")

	node2.addObject("MechanicalObject", template="Rigid3d", position="1 1 1   0 0 0 1", showObject="1")
		
	return root


# Function used only if this script is called from a python environment
if __name__ == '__main__':
    main()