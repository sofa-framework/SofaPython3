import Sofa

g_nb_controllers = 3
g_controllers = []

class TestReassignmentController(Sofa.Core.Controller):

    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)

    def onAnimateBeginEvent(self, event):
        print(f"{self.name.value} : onAnimateBeginEvent")
        pass

    def modifiedAnimateBeginEvent(self, event):
        print(f"{self.name.value} : modifiedAnimateBeginEvent")
        pass

def createScene(root):
    root.dt = 0.01
    root.bbox = [[-1, -1, -1], [1, 1, 1]]
    root.addObject('DefaultVisualManagerLoop')
    root.addObject('DefaultAnimationLoop')

    for i in range(g_nb_controllers):
        controller = root.addObject(TestReassignmentController(name=f"Controller{i}"))
        g_controllers.append(controller)


def main():
    root = Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.initRoot(root)

    # one step with the "fixed" implementation of onAnimateBeginEvent
    Sofa.Simulation.animate(root, root.dt.value) # should print "ControllerX : onAnimateBeginEvent"

    # reassign onAnimateBeginEvent method
    for controller in g_controllers:
        controller.onAnimateBeginEvent = controller.modifiedAnimateBeginEvent

    Sofa.Simulation.animate(root, root.dt.value) # should print "ControllerX : modifiedAnimateBeginEvent"


if __name__ == '__main__':
    main()
