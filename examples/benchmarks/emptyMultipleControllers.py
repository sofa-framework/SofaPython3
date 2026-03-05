import Sofa

g_nb_controllers = 10
g_nb_steps = 10000

class EmptyController(Sofa.Core.Controller):

    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)

    # Default Events *********************************************
    def onAnimateBeginEvent(self, event):  # called at each begin of animation step
        # print(f"{self.name.value} : onAnimateBeginEvent")
        pass

def createScene(root):
    root.dt = 0.01
    root.bbox = [[-1, -1, -1], [1, 1, 1]]
    root.addObject('DefaultVisualManagerLoop')
    root.addObject('DefaultAnimationLoop')


    for i in range(g_nb_controllers):
        root.addObject(EmptyController(name=f"MyEmptyController{i}"))


def main():
    root = Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.initRoot(root)

    # Import the time library
    import time
    start = time.time()
    for iteration in range(g_nb_steps):
        Sofa.Simulation.animate(root, root.dt.value)
    end = time.time()

    print(f"Scene with {g_nb_controllers} controllers and {g_nb_steps} steps took {end - start} seconds.")

    print("End of simulation.")


if __name__ == '__main__':
    main()
