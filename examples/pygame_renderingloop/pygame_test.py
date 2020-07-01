#!/usr/bin/python3

import math
import time
import SofaRuntime
import Sofa.Simulation
import Sofa.Core

class MyC(Sofa.Core.Controller):
        def __init__(self, *args, **kwargs):
                Sofa.Core.Controller.__init__(self, *args, **kwargs)
                self.target = kwargs["target"]
                self.it = 0.0
                
        def onEvent(self, e):
                if e["type"] == "AnimateEndEvent":
                        #print("SIMULATION STEP: " + str(self.target.position.getPathName()))
                        self.target.position.value += math.cos(self.it)
                        self.it += 0.1
                        cameraMVM = self.target.getOpenGLModelViewMatrix()          
                        #print("UPDATE POSITION " + str(self.target.getPathName()))
                        #print("SIMULATION: " + " "+ str(self.target) +"  " + str(cameraMVM))
                
def createScene(root):
        root.addObject("RequiredPlugin", name="SofaComponentAll")
        root.addObject("RequiredPlugin", name="SofaOpenglVisual")
        root.addObject("DefaultAnimationLoop")
        root.addObject("DefaultVisualManagerLoop")

        root.addObject("LightManager")
        root.addObject("SpotLight", position=[0,0,10], direction=[0,0,-1])
        root.addObject("InteractiveCamera", name="camera", position=[0,0,10], 
                        lookAt=[0,0,0], distance=37, fieldOfView=45, zNear=0.63, zFar=55.69)

        root.addObject("MechanicalObject", name="dofs", template="Rigid3", 
                       position=[[0.0, 0.0, 0.0,  0.0, 0.0, 0.0, 1.0]], 
                       showObject=True, showObjectScale=1)
        
        root.addObject("MeshObjLoader", name="loader", filename="mesh/Armadillo_simplified.obj", scale3d="0.1 0.1 0.1")
        root.addObject("OglModel", name="visual", src="@loader", color="1 0 0 0.2")
        
        root.addObject(MyC(target=root.camera))
        return root

if __name__ == "__main__":
        import threading
        from backend_pygame import show

        SofaRuntime.importPlugin("SofaComponentAll")
        
        scene = createScene(Sofa.Core.Node("root"))

        Sofa.Simulation.init(scene)
                
        p1 = show(scene, scene.camera)
        for i in range(1000000):
                for i in range(33):            
                        Sofa.Simulation.animate(scene, 0.01)
                time.sleep(0.0000001)
                
        p1.join()
