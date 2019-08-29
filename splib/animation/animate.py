# -*- coding: utf-8 -*-
"""
Module for a better control over animations in Sofa
------------------------------------------------------------------

splib.animation.Animation
*************************
.. autoclass:: Animation
   :members: __init__

splib.animation.AnimationManager
********************************
.. autoclass:: AnimationManager

splib.animation.AnimationManagerController
******************************************
.. autoclass:: AnimationManagerController(Sofa.Core.Controller)

splib.animation.addAnimation
****************************
.. autofunction:: addAnimation

"""

import Sofa
import Sofa.Core
import Sofa.Types
import Sofa.Simulation
from time import time
from time import sleep

class Animation(object):
    """An animation clip that triggers a callback at regular intervales for a given duration.

    :Example of use:

    .. code-block:: python

        def createScene(rootNode):
            AnimationManager(rootNode)
            def onUpdate(target, factor):
                print("Callback called on: "+target.name+" factor is: "+str(factor))
            def onDone(target, factor):
                print("Callback when animation is done: "+target.name+" factor is: "+str(factor))
            animate(onUpdate, {"target" : rootNode }, 12, onDone=onDone)
    """
    def __init__(self, duration, mode, onUpdate, params, realTimeClock, onDone=None):
        """
        :param str mode: specify how the animation will continue (None, "loop", "pingpong")
        :param float duration: the duration of the animation in seconds.
        :param function onUpdate: callback function called each update.
        :param function onDone: callback function called when the animation is terminated.
        :param dict params: a dictionnary with user specified extra parameters that are passed to the callback.

        """


        if 'startTime' in params:
            self.startTime = params['startTime']
        else:
            self.startTime = None

        self.duration = duration
        self.onUpdate = onUpdate
        self.onDone = onDone
        self.params = params
        self.factor = 1.0
        self.direction = 1.0
        self.mode = mode
        self.realTimeClock = realTimeClock

    def doOnDone(self, currentTime):
        self.onDone(factor=self.factor, **self.params)

    def update(self, currentTime):
        if self.startTime is None:
            self.startTime = currentTime
        if self.duration == 0.0:
            self.factor = 1.0
        elif self.direction  > 0.0:
            self.factor = (currentTime-self.startTime) / self.duration
        else:
            self.factor = 1.0-(currentTime-self.startTime) / self.duration
        if self.factor > 1.0:
            self.factor = 1.0
        if self.factor < 0.0:
            self.factor = 0.0
        self.onUpdate(factor=self.factor, **self.params)



class AnimationManagerController(Sofa.Core.Controller):
    """Implements the AnimationManager as a Controller
    """
    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.listening = True
        self.name = "AnimationManager"
        self.totalTime = 0
        self.animations = []
        self.currentTime = 0
        self.realTimeAnimations = []
        self.realTotalTime = 0
        self.begin = time()


    def addAnimation(self, animation):
        if (animation.realTimeClock):
            self.realTimeAnimations.append(animation)
        else:
            self.animations.append(animation)

    def removeAnimation(self, animation):
        if (animation.realTimeClock):
            self.realTimeAnimations.remove(animation)
        else:
            self.animations.remove(animation)

    def bwdInitGraph(self, root):
        self.onAnimateBeginEvent(0.0)

    def onAnimateBeginEvent(self, event):
        self.totalTime = self.totalTime + float(event.get('dt', None))
        self.realTotalTime = time() - self.begin

        self.animations = computeAnimations(self.animations, self.totalTime)
        self.realTimeAnimations = computeAnimations(self.realTimeAnimations, self.realTotalTime)
        return 0

def computeAnimations(animations, totalTime):
    nextanimations = []
    for animation in animations:
        animation.update(totalTime)
        if animation.factor < 1.0 and animation.direction > 0.0:
            nextanimations.append(animation)
        elif animation.factor > 0.0 and animation.direction < 0.0:
            nextanimations.append(animation)
        elif animation.mode == "pingpong":
            animation.direction = -animation.direction
            animation.startTime = None
            nextanimations.append(animation)
        elif animation.mode == "loop":
            animation.direction = animation.direction
            animation.startTime = None
            nextanimations.append(animation)
        elif animation.onDone is not None:
            animation.doOnDone(totalTime)
    return nextanimations

manager = None

def addAnimation(onUpdate, params, duration, mode="once", onDone=None, realTimeClock=False):
    """Construct and add an animation

    Build a new animation from a callback function that computes the animation value,
    a set of parameters, the animation duration and the type of animation repetition pattern.

    Animation can be added from any code location (createScene, PythonScriptController)

    :param function onUpdate: the function to execute at every step of the simulation
    :param dict params: a dictionnary with user specified extra parameters
    :param float duration: duration of the animation in seconds.
    :param str mode: once, loop, pingpong
    :param function onDone: the callback function to execute at the end of the animation
    :param bool realTimeClock: sets the clock of the animation to machine time or simulation time

    :Example of use:

    .. image:: ../../../../images/realTimeClockScene.gif
            :alt: Animation of the example scene
            :align: center
            :height: 250pt

    This example creates two spheres, with one moving in machine time and the other in simulated time.

    .. sourcecode:: python

        import Sofa.Core
        from splib.animation import AnimationManager, addAnimation
        from splib.animation import easing

        def myAnimate(target, factor):
                with target.mstate.position.writeable() as tr:
                        tr[0][0] = -20 + easing.sineInOut(factor)*20
                target.mstate.init()

        def Sphere(rootNode, name, position, color):
                #Creating the sphere
                sphere = rootNode.addChild(name)
                sphere.addObject('MechanicalObject', name="mstate", template="Rigid3",
                    position=position)
                #### Visualization of the sphere
                sphereVisu = sphere.addChild("VisualModel")
                sphereVisu.loader = sphereVisu.addObject('MeshObjLoader', name="loader",
                    filename="mesh/ball.obj", scale=0.5)
                sphereVisu.addObject('OglModel', name="model", src="@loader", color=color)
                sphereVisu.addObject('RigidMapping')
                return sphere

        def createScene(rootNode):
                confignode = rootNode.addChild("Config")
                confignode.addObject('RequiredPlugin', name="SofaPython3", printLog=False)
                manager = AnimationManager(rootNode)
                rootNode.addObject(manager)
                sphere1 = Sphere(rootNode, "sphere1", [-20,0,0,0,0,0,1],[1/10.0,1*0.7/10.0,0.9])
                sphere2 = Sphere(rootNode, "sphere2", [-20,1,0,0,0,0,1],[7/10.0,7*0.7/10.0,0.9])
                addAnimation(myAnimate, {"target": sphere1}, 5, mode="pingpong")
                addAnimation(myAnimate, {"target": sphere2}, 5, mode="pingpong",
                    realTimeClock=True)
                return rootNode

    """
    if manager == None:
        raise Exception("Missing manager in this scene")
    manager.addAnimation(Animation(duration=duration, mode=mode, onUpdate=onUpdate, params=params, realTimeClock=realTimeClock, onDone=onDone))

def AnimationManager(node):
    """
    A Controller to manage all animations in the scene

    Before using the animation framework an AnimationManager
    must be added to the scene. It has in charge, at each time step
    to update all the running animations.

    :Returns:
        AnimationManagerController

    :Example of use:
        .. sourcecode:: python

            def createScene(rootNode)
                AnimationManager(rootNode)
    """
    global manager
    if manager is not None:
        print("There is already one animation manager in this scene...why do you need a second one ?")
        return manager
    manager = AnimationManagerController(name="AnimationController")
    node.addObject(manager)
    return manager

# This function is just an example on how to use the addAnimation function.
def createScene(rootNode):

    def myAnimate1(target, factor):
        print("I should do something on: "+target.name.value+" factor is: "+str(factor))

    def myAnimate2(target, factor):
        print("Function 2: "+target.name.value+" factor is: "+str(factor))

    def myOnDone(target, factor):
        print("onDone: "+target.name.value+" factor is: "+str(factor))

    animation.AnimationManager(rootNode)
    animation.addAnimation(myAnimate1, {"target": rootNode}, 10)
    animation.addAnimation(myAnimate2, {"target": rootNode}, 12, onDone=myOnDone)
