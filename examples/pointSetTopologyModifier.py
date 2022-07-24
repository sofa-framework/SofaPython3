import Sofa
import Sofa.Core
import numpy as np


def createScene(root):
    root.gravity = [0, -9.81, 0]

    root.addObject("DefaultAnimationLoop")
    root.addObject("DefaultVisualManagerLoop")
    root.addObject("RequiredPlugin", name="Sofa.Component.Topology.Container.Dynamic")

    container = root.addObject("PointSetTopologyContainer", points=[[0, 0, 0], [1, 0, 0]])
    modifier = root.addObject("PointSetTopologyModifier")
    state = root.addObject("MechanicalObject", template="Vec3d", showObject=True, showObjectScale=10)

    root.addObject(PointController(modifier=modifier, state=state, container=container))


class PointController(Sofa.Core.Controller):
    def __init__(self, modifier, state, container):
        super().__init__()
        self.container = container
        self.modifier = modifier
        self.state = state

    def onKeypressedEvent(self, event):
        if event["key"] == "A":
            print("Add 10 points")
            self.modifier.addPoints(10, True)

            with self.state.position.writeable() as state:
                for i in range(len(state)):
                    state[i] = np.array([i, 0, 0])

        elif event["key"] == "D":
            print("Remove point 0")
            self.modifier.removePoints(np.array([0]), True)
