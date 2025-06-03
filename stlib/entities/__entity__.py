from stlib.core.baseParameters import BaseParameters
from stlib.prefabs.collision import CollisionParameters, Collision
from stlib.prefabs.visual import VisualParameters, Visual
from stlib.prefabs.behavior import Behavior, BehaviorParameters
from stlib.geometry import Geometry
from stlib.geometry.extract import ExtractParameters
import dataclasses
from typing import Callable, Optional, overload, Any
from stlib.geometry import GeometryParameters
from splib.core.enum_types import StateType
import Sofa


@dataclasses.dataclass
class EntityParameters(BaseParameters): 
    name = "Entity"

    template : StateType

    ### QUID 
    addCollision : Optional[Callable] = lambda x : Collision(CollisionParameters())
    addVisual : Optional[Callable] = lambda x : Visual(VisualParameters()) 

    geometry : GeometryParameters
    behavior : BehaviorParameters
    collision : Optional[CollisionParameters] = None
    visual : Optional[VisualParameters] = None
    


class Entity(Sofa.Core.BaseEntity): 

    # A simulated object
    behavior : Behavior
    visual : Visual
    collision : Collision
    geometry : Geometry
    
    parameters : EntityParameters
    

    def __init__(self, parent=None, parameters=EntityParameters(), **kwargs):
        Sofa.Core.Node.__init__(self, name=parameters.name)        

        if parent is not None: 
            parent.addChild(self)
        
        self.parameters = parameters

        self.geometry = self.add(Geometry, self.parameters.geometry)

        ### Check compatilibility of Behavior
        if self.parameters.behavior.stateType != self.parameters.template:
            print("WARNING: imcompatibility between templates of entity and behavior")
            self.parameters.behavior.stateType = self.parameters.template

        self.behavior = self.add(Behavior,self.parameters.behavior)
        
        if self.parameters.collision is not None:
            self.collision = self.add(Collision,self.parameters.collision)
            self.addMapping(self.parameters.collision, self.collision)

        
        if self.parameters.visual is not None:
            self.visual = self.add(Visual,self.parameters.visual)
            self.addMapping(self.parameters.visual, self.visual)

    def addMapping(self, destParameter, destPrefab):
        if( self.parameters.template == StateType.VEC3):
            if isinstance(destParameter.geometry,ExtractParameters):
                destPrefab.addObject("IdentityMapping", input="@../behavior/", output="@.", template='Vec3,Vec3')
            else :
                destPrefab.addObject("BarycentricMapping", input="@../behavior/", output="@.", template='Vec3,Vec3')
        else:
            destPrefab.addObject("RigidMapping", input="@../behavior", output="@.", template='Rigid3,Vec3')
