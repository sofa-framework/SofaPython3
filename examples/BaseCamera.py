import Sofa.Core
from Sofa.constants import *
from random import randint
from splib.animation import easing
import math
import numpy

def eulerToQuat(a):
    """ This function takes an array of Euler Angles as argument and returns a Quaternion
    """
    yaw, pitch, roll = a
    cy = math.cos(yaw * 0.5);
    sy = math.sin(yaw * 0.5);
    cp = math.cos(pitch * 0.5);
    sp = math.sin(pitch * 0.5);
    cr = math.cos(roll * 0.5);
    sr = math.sin(roll * 0.5);

    q = [0 for k in range(4)]
    q[3] = cy * cp * cr + sy * sp * sr;
    q[0] = cy * cp * sr - sy * sp * cr;
    q[1] = sy * cp * sr + cy * sp * cr;
    q[2] = sy * cp * cr - cy * sp * sr;

    return q


class KeyPressedController(Sofa.Core.Controller):
    """ This controller monitors key and Mouse movements.
    """
    def __init__(self, *args, **kwargs):
        print(kwargs)
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.listening = True
        self.name = "keyPressedController"
        self.camera = kwargs.get("camera", None)
        self.mousePositionOnClick=[0,0]
        self.hasClicked = False

    def onKeypressedEvent(self, event):
        N = 100
        if event['key'] == Key.Z:
            for i in range(1,N):
                self.camera.position[2] += - 0.5*(easing.sineInOut(i/(N-1))- easing.sineInOut((i-1)/(N-1)))
        elif event['key'] == Key.Q:
            for i in range(1,N):
                self.camera.position[0] += - 0.5*(easing.sineInOut(i/(N-1))- easing.sineInOut((i-1)/(N-1)))
        elif event['key'] == Key.D:
            for i in range(1,N):
                self.camera.position[0] += 0.5*(easing.sineInOut(i/(N-1))- easing.sineInOut((i-1)/(N-1)))
        elif event['key'] == Key.X:
            for i in range(1,N):
                self.camera.position[2] += 0.5*(easing.sineInOut(i/(N-1))- easing.sineInOut((i-1)/(N-1)))
        else :
            print("")

    def onMouseEvent(self, event):
        if (event['State']==1):
            self.mousePositionOnClick[0] = event['mouseX']
            self.mousePositionOnClick[1] = event['mouseY']
            self.hasClicked = True
        if (event['State']== 0):
            if (self.hasClicked):
                euler = [ 0.0, (event['mouseX'] - self.mousePositionOnClick[0])*0.001, (event['mouseY'] - self.mousePositionOnClick[1])*0.001]
                quat = eulerToQuat(euler)
                self.camera.rotate(quat)
            self.mousePositionOnClick[0] = event['mouseX']
            self.mousePositionOnClick[1] = event['mouseY']
        if (event['State']==2):
            self.hasClicked = False
            

def Sphere(rootNode, name, position, color):
    #Creating the sphere
    sphere = rootNode.addChild(name)
    sphere.addObject('MechanicalObject', name="mstate", template="Rigid3", position=position)

    #### Visualization of the sphere
    sphereVisu = sphere.addChild("VisualModel")
    sphereVisu.loader = sphereVisu.addObject('MeshObjLoader', name="loader", 
                                              filename="mesh/ball.obj", scale=0.5)
    sphereVisu.addObject('OglModel', name="model", src="@loader", color=color)
    sphereVisu.addObject('RigidMapping')
    return sphere

def createScene(rootNode):
    """ This scene is an example scene to demonstrate an implementation of a Controller for the Camera in runSofa. 
        To test this, in runSofa, you must put the View parameter (top left of the runSofa window) to OpenGL
        Then click on Animate, and move the Camera using Ctrl + z, q, d and x, orientate it by clicking and moving with the Mouse!
    """
    for i in range(30):
        sphere = Sphere(rootNode, "sphere"+str(i), [randint(0,6),randint(0,6),randint(0,6),0,0,0,1],[i/40.0,i*0.7/40.0,0.9])
    rootNode.addObject("Camera", name = "camera")
    rootNode.addObject(KeyPressedController(name = "MyController", camera = rootNode.camera))
    return rootNode
