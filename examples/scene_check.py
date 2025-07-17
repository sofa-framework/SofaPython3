import Sofa
import SofaRuntime

def main():
    # Make sure to load all necessary libraries
    SofaRuntime.importPlugin("Sofa.Component.StateContainer")
    # This is important to register the scene checks contained in this plugin into the registry
    SofaRuntime.importPlugin("SceneChecking")

    for scene_check in Sofa.Simulation.SceneCheckMainRegistry.getRegisteredSceneChecks():
        print(f"{scene_check.getName()}: {scene_check.getDesc()}")

    # Call the above function to create the scene graph
    root = Sofa.Core.Node("root")
    createScene(root)

    # Once defined, initialization of the scene graph
    Sofa.Simulation.initRoot(root)

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

if __name__ == '__main__':
    main()
