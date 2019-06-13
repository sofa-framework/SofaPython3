#!/usr/bin/python3

import Sofa
import SofaRuntime

_runAsPythonScript = False


class RotationController(Sofa.Controller):
    """ This is a custom controller to perform actions when events are triggered """

    def __init__(self, *args, **kwargs):
        # These are needed (and the normal way to override from a python class)
        Sofa.Controller.__init__(self, *args, **kwargs)
        print(" Python::__init__::" + str(self.name.value))

        self.engine = kwargs["engine"]

        self.inited = 0
        self.iterations = 0
        self.step = 1
        self.max_iterations = 360
        self.other_direction = False

    def onAnimateBeginEvent(self, eventType):
        self.iterations += 1
        self.engine.rotation.value[0] += self.step if (
            self.other_direction) else -self.step
        if self.iterations > self.max_iterations:
            self.iterations = 0
            self.other_direction = not self.other_direction


def createScene(root):

    rpath = "/home/bruno/dev/sofa/share/mesh/"

    # can be used by SceneLoaderPY:
    root.dt = 0.01
    root.name = 'root'
    root.gravity = [0.0, 9.8, 0.0]

    loader = root.addObject('MeshObjLoader', name='loader',
                            filename=rpath + "liver.obj")
    te = root.addObject(
        "TransformEngine", name="te", input_position=loader.position.getLink())
    mo = root.addObject("MechanicalObject", name="mo",
                        position=te.output_position.getLink())

    visu = root.addChild("Visu")
    visu.addObject('OglModel', name="visu", src=loader.getLink())
    visu.addObject('IdentityMapping', name="BM", src=mo.getLink())

    if not _runAsPythonScript:
        root.addObject(RotationController(name="MyController", engine=root.te))


def main():
    # can be executed from terminal directly:

    # Register all the common component in the factory.
    SofaRuntime.importPlugin("SofaAllCommonComponents")

    _runAsPythonScript = True
    root = Sofa.Node()

    createScene(root)
    Sofa.Simulation.init(root)
    for i in range(0, 360):
        Sofa.Simulation.animate(root, root.dt.value)
        root.te.rotation.value[0] += 1


if __name__ == '__main__':
    main()
