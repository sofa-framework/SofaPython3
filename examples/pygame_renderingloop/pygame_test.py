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
        root.addObject("RequiredPlugin", name="SofaAllCommonComponents")
        root.addObject("RequiredPlugin", name="SofaOpenglVisual")
        root.addObject("DefaultAnimationLoop")
        root.addObject("DefaultVisualManagerLoop")
        #root.addObject("MeshObjLoader", name="loader", filename="mesh/Armadillo_simplified.obj")
        root.addObject("MechanicalObject", name="dofs", template="Rigid3", 
                       position=[[0.0,1.0,0.0,1.0,1.0,0.0,1.0]], 
                       showObject=True, showObjectScale=20)
        
        #root.addObject("OglModel", name="visual", src=root.loader.getLink())
        #root.addObject("RigidMapping", name="mapping", input=root.dofs.getLink(), output=root.visual.getLink())
        root.addObject("LightManager")
        root.addObject("SpotLight", position=[-7.0,0.3,20])
        root.addObject("InteractiveCamera", name="camera", position=[-10.43,33,19], orientation=[-0.25,0.43, 0.84, 0.177], 
                        lookAt=[0.15,2.4,1.31], distance=37, fieldOfView=45, zNear=0.63, zFar=55.69)
        
        root.addObject(MyC(target=root.camera))
        return root

if __name__ == "__main__":
        import threading
        from backend_pygame import show

        SofaRuntime.importPlugin("SofaAllCommonComponents")
        
        scene = createScene(Sofa.Core.Node("root"))

        Sofa.Simulation.init(scene)
                
        p1 = show(scene, scene.camera)
        for i in range(1000000):
                for i in range(33):            
                        Sofa.Simulation.animate(scene, 0.01)
                time.sleep(0.0000001)
                
        p1.join()
