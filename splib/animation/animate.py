# -*- coding: utf-8 -*-
import Sofa
import Sofa.Core
import Sofa.Types
import Sofa.Simulation
from time import time
from time import sleep

class Animation(object):
    """An animation clip that trigger callback at regular intervales for a given duration.

           :param mode: specify how the animation will continue (None, "loop", "pingpong")
           :param duration: the duration of the animation in seconds.
           :param onUpdate: callback function called each update.
           :param onDone: callback function called when the animation is terminated.
           :param params: a dictionnary with user specified extra parameters that are passed to the callback.

    Example of use:


    .. code-block:: python

        def createScene(rootNode):
            AnimationManager(rootNode)

            def onUpdate(target, factor):
                print("Callback called on: "+target.name+" factor is: "+str(factor))

            def onDone(target, factor):
                print("Callback when animation is done: "+target.name+" factor is: "+str(factor))

            animate(onUpdate, {"target" : rootNode }, 12, onDone=onDone)
    """
    def __init__(self, duration, mode, onUpdate, params, onDone=None):
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

    def doOnDone(self, currentTime):
        self.onDone(factor=self.factor, **self.params)

    def update(self, currentTime):
        if self.startTime is None:
            self.startTime = currentTime

        if self.duration == 0.0:
            self.factor = 1.0
        elif self.direction > 0.0:
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
        self.realTimeStep = None
        self.realTimeClock= kwargs.get('realTimeClock', False)
        self.begin = time()


    def addAnimation(self, animation):
        self.animations.append(animation)

    def removeAnimation(self, animation):
        self.animations.remove(animation)

    def bwdInitGraph(self, root):
        self.onAnimateBeginEvent(0.0)

    def onAnimateBeginEvent(self, event):
        if self.realTimeStep is not None:
            self.currentTime = time()
        if (self.realTimeClock):
            self.totalTime = time() - self.begin
        else:
            self.totalTime = self.totalTime + float(event.get('dt', None))
        nextanimations = []
        for animation in self.animations:
            animation.update(self.totalTime)
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
                animation.doOnDone(self.totalTime)
        self.animations = nextanimations
        return 0

    def onAnimateEndEvent(self, event):
        timeLeft = self.realTimeStep - (time() - self.currentTime)
        if self.realTimeStep is not None:
            sleep(timeLeft)


manager = None

def addAnimation(onUpdate, params, duration, mode="once", onDone=None):
    """Construct and add an animation

    Build a new animation from a callback function that computes the animation value,
    a set of parameters, the animation duration and the type of animation repetition pattern.

    Animation can be added from any code location (createScene, PythonScriptController)

    :param float duration: duration of the animation in seconds.
    :param str mode: once, loop, pingpong

    Example:
        .. sourcecode:: python

            def myAddAnimation(target, factor):
                print("I should do something on: "+target.name)


            def createScene(rootNode)
                AnimationManager(rootNode)
                addAnimation(myAddAnimation, {"target" : rootNode }, 10)
    """
    if manager == None:
        raise Exception("Missing manager in this scene")
    manager.addAnimation(Animation(duration=duration, mode=mode, onUpdate=onUpdate, params=params, onDone=onDone))

def realTimeAnimation(node, step, totalDuration):
    """
    Start the animations stored in the AnimationManagerController,
    and runs it in real time.

    This function will run in real time only if the time of computation of a step is inferieur to the duration of a step.

    :param Node node: the node to animate
    :param float step: the duration of a step (in seconds)
    :param float totalDuration: the total duration of the simulation (in seconds)

    Example:
        def createScene(rootNode):

            manager = animation.AnimationManager(rootNode, 0.01)

            def myAnimate1(target, factor):
                print("I should do something on: "+target.name.value+" factor is: "+str(factor))

            def myAnimate2(target, factor):
                print("Function 2: "+target.name.value+" factor is: "+str(factor))

            def myOnDone(target, factor):
                print("onDone: "+target.name.value+" factor is: "+str(factor))

            animation.addAnimation(myAnimate1, {"target": rootNode}, 1)
            animation.addAnimation(myAnimate2, {"target": rootNode}, 3, onDone=myOnDone)

            Sofa.Simulation.init(rootNode)
            animation.realTimeAnimation(rootNode,0.5,5)
    """
    begin = time()
    while (time() - begin < totalDuration):
        Sofa.Simulation.animate(node, step)
    return 0

def removeAnimation(animation):
    if manager == None:
        raise Exception("Missing manager in this scene")

    manager.removeAnimation(animation)

def AnimationManager(node, realTimeStep=None, realTimeClock=False):
    """
    A Controller to manage all animations in the scene

    Before using the animation framework an AnimationManager
    must be added to the scene. It has in charge, at each time step
    to update all the running animations.

    Returns:
        AnimationManagerController

    Example:
        .. sourcecode:: python

            def createScene(rootNode)
                AnimationManager(rootNode)
    """
    global manager
    if manager is not None:
        print("There is already one animation manager in this scene...why do you need a second one ?")
        return manager
    manager = AnimationManagerController( realTimeClock=realTimeClock, name="AnimationController")
    manager.realTimeStep = realTimeStep
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
