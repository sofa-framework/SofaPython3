import Sofa 
from stlib.core.basePrefab import BasePrefab
from cristal.prefabs.interactivecamera.__parameters__ import *
    
class InteractiveCamera(BasePrefab):
    class ProjectionType:
        Perspective = 0
        Orthographic = 1

    def __init__(self, params : InteractiveCameraParameters = None, **kwargs):
        if params is None:
            params = InteractiveCameraParameters(**kwargs)

        BasePrefab.__init__(self, params=params)
        self.__create_prefab__()

    def __create_prefab__(self):
        camera = self.add("InteractiveCamera", name="state")    
        self.add(InteractiveCameraController, name="controller", camera=camera)    

class InteractiveCameraController(Sofa.Core.Controller):
    def __init__(self, parameters : InteractiveCameraControllerParameters = None, **kwargs):
        if parameters is None:
            parameters = InteractiveCameraControllerParameters(**kwargs)
        
        Sofa.Core.Controller.__init__(self, **BaseParameters.fromParameters(parameters))
        self.camera = parameters.camera 

    def onKeyreleasedEvent(self, event):
        key = event["key"]

        modes = ["Look-at position", "Camera position", 
                 "Orientation in local coordinates", "Orientation in world coordinate"]
        # 0: control look-at 
        # 1: control position
        # 2: control orientation in scene coordinate 
        # 3: control orientation in world coordinate
        if key == "M":        
            self.camera.pivot = (self.camera.pivot.value+1) % 4
            print(f"Switching to camera mode: {modes[self.camera.pivot.value]}")
        elif key == "L":
            self.camera.pivot = (self.camera.pivot.value-1) % 4
            print(f"Switching to camera mode: {modes[self.camera.pivot.value]}")
        elif key == "0":
            print("SHOULD MOVE AXIS ALIGNED 0")
        elif key == "1":
            print("SHOULD MOVE AXIS ALIGNED 1")
        elif key == "3":
            print("SHOULD MOVE AXIS ALIGNED 2")
        elif key == "7":
            print("SHOULD MOVE AXIS ALIGNED 7")
        elif key == "P": #< switch to/from orthographic mode
            if self.camera.projectionType.value == InteractiveCamera.ProjectionType.Orthographic: 
                self.camera.projectionType = InteractiveCamera.ProjectionType.Perspective 
            else:
                self.camera.projectionType = InteractiveCamera.ProjectionType.Orthographic
        else:
            return False 

        return True 
             