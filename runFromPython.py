# encoding: utf-8
#!/usr/bin/python3
import sys
import os

if "SOFA_ROOT" not in os.environ:
        print("WARNING: missing SOFA_ROOT in you environment variable. ") 
        sys.exit(-1)

sys.path.append(os.path.abspath("./bindings/Sofa/package"))
sys.path.append(os.path.abspath("./bindings/SofaRuntime/package"))
sys.path.append(os.path.abspath("./bindings/SofaTypes/package"))

import Sofa
import SofaRuntime

## Register all the common component in the factory. 
SofaRuntime.importPlugin("SofaAllCommonComponents")

class MyController(Sofa.Controller):
        def __init__(self, *args, **kwargs):
                Sofa.Controller.__init__(self,*args, **kwargs)
                print("INITED")
                
        def onEvent(self, event):
                print("Event: "+event)
               
                
root = Sofa.Node("myroot")
root.addChild("child1")
root.addObject(MyController())

Sofa.Simulation.init(root)
Sofa.Simulation.print(root)
for i in range(0, 10):
        print("step: "+str(i))
        Sofa.Simulation.animate(root, 0.1)
        
        
