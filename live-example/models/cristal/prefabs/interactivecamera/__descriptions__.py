from stlib.core.basePrefab import BasePrefab
import Sofa

class InteractiveCamera: ...
class InteractiveCameraController: ...

class InteractiveCamera(BasePrefab):
    state : Sofa.Core.Object
    controller : InteractiveCameraController 
    
class InteractiveCameraController(Sofa.Core.Controller):
    camera : InteractiveCamera
