#!/usr/bin/python3

import Sofa.Core
import Sofa.Simulation
import SofaRuntime
import os

_runAsPythonScript = False


class RotationController(Sofa.Core.Controller):
    """ This is a custom controller to perform actions when events are triggered """

    def __init__(self, *args, **kwargs):
        # These are needed (and the normal way to override from a python class)
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        print(" Python::__init__::" + str(self.name.value))

        self.engine = kwargs["engine"]

        self.inited = 0
        self.axe = 0 
        self.iterations = 0
        self.step = 1
        self.max_iterations = 360
        self.other_direction = False
        self.move = 0

    def rotate(self):
        self.iterations += 1
        self.engine.rotation[self.axe] += self.step if (
            self.other_direction) else -self.step
        if self.iterations > self.max_iterations:
            self.iterations = 0
            self.other_direction = not self.other_direction

    def translate(self):
        self.iterations += 1
        self.engine.translation[self.axe] += 0.05 if (
            self.other_direction) else -0.05
        if self.iterations > self.max_iterations:
            self.iterations = 0
            self.other_direction = not self.other_direction

    def onAnimateBeginEvent(self, eventType):
        if(self.move == 0):
            self.rotate()
        elif (self.move == 1):
            self.translate()
        

    def onEvent(self, event):
        pass
        # print ("Different event in Sofa : "+str(event))

    def onKeypressedEvent(self, c):
        key = c['key']
        if key == "0":
            print("You switch to X axis")
            self.axe = 0
        if key == "1":
            print("You switch to Y axis")
            self.axe = 1
        if key == "2":
            print("You switch to Z axis")
            self.axe = 2

        if ord(key) == 19:  # up
            print("You just switch to rotation control ")
            self.move = 0 

        if ord(key) == 21:  # down
            print("You just switch to rotation control ")
            self.move = 0 

        if ord(key) == 18:  # left
            print("You just switch to rotation control ")
            self.move = 1

        if ord(key) == 20:  # right
            print("You just switch to rotation control ")
            self.move = 1

def createScene(root):

    # rpath =os.environ["SOFA_ROOT"]+"../src/share/mesh/"
    # can be used by SceneLoaderPY:
    root.dt = 0.01
    root.name = 'root'
    root.gravity = [0.0, 9.8, 0.0]

    loader = root.addObject('MeshObjLoader', name='loader',
                            filename="mesh/liver.obj")
    te = root.addObject(
        "TransformEngine", name="te", input_position=loader.position.getLinkPath(), rotation=[0,0,0])
    mo = root.addObject("MechanicalObject", name="mo",
                        position=te.output_position.getLinkPath())

    visu = root.addChild("Visu")
    visu.addObject('OglModel', name="visu", src=loader.getLinkPath())
    visu.addObject('IdentityMapping', name="BM", src=mo.getLinkPath())

    if not _runAsPythonScript:
        root.addObject(RotationController(name="MyController", engine=root.te))


def main():
    # can be executed from terminal directly:

    # Register all the common component in the factory.
    SofaRuntime.importPlugin("SofaComponentAll")
    SofaRuntime.importPlugin("SofaOpenglVisual")
    global _runAsPythonScript
    _runAsPythonScript = True
    root = Sofa.Core.Node()

    createScene(root)
    Sofa.Simulation.init(root)
    for i in range(0, 360):
        Sofa.Simulation.animate(root, root.dt.value)
        root.te.rotation[0] += 1
        #print("For i = "+ str(i)+", we have : "+str(root.te.rotation.value[0]))
    print("Last value is : "+ str(root.te.rotation.value[0]))

if __name__ == '__main__':
    main()
