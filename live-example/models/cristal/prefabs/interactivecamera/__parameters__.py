from stlib.core.baseParameters import BaseParameters, dataclasses
from cristal.prefabs.interactivecamera.__descriptions__ import *

@dataclasses.dataclass
class InteractiveCameraControllerParameters(BaseParameters):
    camera : InteractiveCamera = None 
     
@dataclasses.dataclass
class InteractiveCameraParameters(BaseParameters):
    state : Sofa.Core.Camera = None