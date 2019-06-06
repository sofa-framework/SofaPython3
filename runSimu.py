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

if len(sys.argv) != 2:
        print("USAGE: python3 runSimu.py scene.py")
        sys.exit(-1)


s = SofaRuntime.load(sys.argv[1])
s.init()
for i in range(10):
        SofaRuntime.getSimulation().animate(s, 0.1)

        
        
        
