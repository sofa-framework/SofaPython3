#!/usr/bin/python3

import Sofa.Core
import Sofa.Simulation
import SofaRuntime
import os
import Sofa.splib.animation
from time import time
SofaRuntime.importPlugin("SofaOpenglVisual")

_runAsPythonScript = False


class RotationController(Sofa.Core.Controller):
    """ This is a custom controller to perform actions when events are triggered """

    def __init__(self, *args, **kwargs):
        # These are needed (and the normal way to override from a python class)
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
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

    rpath =os.environ["SOFA_ROOT"]+"../src/share/mesh/"
    root.dt = 0.5
    root.name = 'root'
    root.gravity = [0.0, 9.8, 0.0]

    loader = root.addObject('MeshObjLoader', name='loader',
                            filename=rpath + "liver.obj")
    te = root.addObject(
        "TransformEngine", name="te", input_position=loader.position.getLinkPath(), rotation=[0,0,0])
    mo = root.addObject("MechanicalObject", name="mo",
                        position=te.output_position.getLinkPath())

    visu = root.addChild("Visu")
    visu.addObject('OglModel', name="visu", src=loader.getLinkPath())
    visu.addObject('IdentityMapping', name="BM", src=mo.getLinkPath())

    Sofa.Simulation.init(root)
    # First, we simulate in simulated time :
    print("Simulated : ")
    begin = time()
    for i in range(0, 302):
        Sofa.Simulation.animate(root, root.dt.value)
        root.te.rotation[0] += 1
        #print("New rotation : "+str(root.te.rotation[0]))
    print("The final angle is : "+str(root.te.rotation[0]))
    print("Duration of the simulation : "+str(time() - begin))
    root.te.rotation[0] = 0

    def myAnimate(target, factor):
        #print("New rotation : "+str(target.rotation[0])+" factor is: "+str(factor))
        target.rotation[0] += 1

    def myOnDone(target, factor):
        print("The final angle is : " +str(target.rotation[0]))
        print("Duration of the simulation : "+str(time() - begin))
    # Now, we simulate in real time, the real time step is here 0.01
    manager = Sofa.splib.animation.AnimationManager(root, 0.01)

    Sofa.splib.animation.addAnimation(myAnimate, {"target": root.te}, 3, onDone=myOnDone)
    Sofa.Simulation.init(root)
    print("Real time : ")
    begin = time()
    Sofa.splib.animation.realTimeAnimation(root,0.01,4)
    if not _runAsPythonScript:
        root.addObject(RotationController(name="MyController", engine=root.te))


def main():
    # can be executed from terminal directly:

    # Register all the common component in the factory.
    SofaRuntime.importPlugin("SofaAllCommonComponents")
    SofaRuntime.importPlugin("SofaOpenglVisual")
    global _runAsPythonScript
    _runAsPythonScript = True
    root = Sofa.Core.Node("root")
    createScene(root)


if __name__ == '__main__':
    main()
