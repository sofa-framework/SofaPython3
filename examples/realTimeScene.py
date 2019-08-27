#!/usr/bin/python3

import Sofa.Core
import Sofa.Simulation
import SofaRuntime
import os
import splib.animation
from time import time


def simulatedTimeLoop(root):
    """ Here we simulate in simulated time :
    """
    print("Simulated : ")
    begin = time()
    for i in range(0, 302):
        Sofa.Simulation.animate(root, root.dt.value)
        root.te.rotation[0] += 1
        #print("New rotation : "+str(root.te.rotation[0]))
    print("The final angle is : "+str(root.te.rotation[0]))
    print("Duration of the simulation : "+str(time() - begin))


def realTimeLoop(root):
    """ realTimeAnimation starts a loop, that here lasts 4 seconds and its steps last 0.01 
    (we make this happen by measuring the time between the beginning and the end of the step,
    comparing it to the wanted duration of the step and waiting if there is still time left to 
    be spent on the step. 
    """
    def myAnimate(target, factor):
        #print("New rotation : "+str(target.rotation[0])+" factor is: "+str(factor))
        target.rotation[0] += 1

    def myOnDone(target, factor):
        print("The final angle is : " +str(target.rotation[0]))
        print("Duration of the simulation : "+str(time() - begin))
    # Now, we simulate in real time, the real time step is here 0.01
    manager = splib.animation.AnimationManager(root, 0.01)

    splib.animation.addAnimation(myAnimate, {"target": root.te}, 3, onDone=myOnDone)
    print("Real time : ")
    begin = time()

    splib.animation.realTimeAnimation(root,0.01,4)

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
    
    simulatedTimeLoop(root)
    
    root.te.rotation[0] = 0
    
    realTimeLoop(root)



if __name__ == '__main__':
    main()
