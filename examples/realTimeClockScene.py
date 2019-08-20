#!/usr/bin/python3

import Sofa.Core
import Sofa.Simulation
import SofaRuntime
import os
import Sofa.splib.animation
from time import time
g_keepAnimating = True

def simulatedTimeLoop(root):
    """ Here we simulate in simulated time
    """
    def myAnimate(target, factor):
        #print("New rotation : "+str(target.rotation[0])+" factor is: "+str(factor))
        target.rotation[0] += 1

    def myOnDone(target, factor):
        global g_keepAnimating
        g_keepAnimating = False
        print(g_keepAnimating)
        print("The final angle is : " +str(target.rotation[0]))
        print("Duration of the simulation : "+str(time() - begin))

    manager = Sofa.splib.animation.AnimationManager(root)

    Sofa.splib.animation.addAnimation(myAnimate, {"target": root.te}, 3, onDone=myOnDone)
    print("Simulated time : ")
    begin = time()
    while(g_keepAnimating):
        Sofa.Simulation.animate(root,0.0)

    return manager

def realTimeClockLoop(root, manager):
    """ Here we simulate in machine time, by setting the realTimeClock of the manager to True
    """
    def myAnimate(target, factor):
        #print("New rotation : "+str(target.rotation[0])+" factor is: "+str(factor))
        target.rotation[0] += 1

    def myOnDone(target, factor):
        global g_keepAnimating
        g_keepAnimating = False
        print("The final angle is : " +str(target.rotation[0]))
        print("Duration of the simulation : "+str(time() - begin))
    manager.realTimeClock = True

    Sofa.splib.animation.addAnimation(myAnimate, {"target": root.te}, 3, onDone=myOnDone)
    print("Real time : ")
    begin = time()
    while (g_keepAnimating):
        Sofa.Simulation.animate(root,0.0)

def main():
    
    rpath =os.environ["SOFA_ROOT"]+"../src/share/mesh/"

    # Register all the common component in the factory.
    SofaRuntime.importPlugin("SofaAllCommonComponents")
    
    root = Sofa.Core.Node("root")
    Sofa.Simulation.init(root)
    loader = root.addObject('MeshObjLoader', name='loader',
                            filename=rpath + "liver.obj")
    te = root.addObject(
        "TransformEngine", name="te", input_position=loader.position.getLinkPath(), rotation=[0,0,0])
    mo = root.addObject("MechanicalObject", name="mo", position=te.output_position.getLinkPath())

    manager = simulatedTimeLoop(root)    
    
    root.te.rotation[0] = 0
    global g_keepAnimating
    g_keepAnimating = True
    realTimeClockLoop(root, manager)


if __name__ == '__main__':
    main()
