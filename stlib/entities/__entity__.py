from stlib.core.baseParameters import BaseParameters
from stlib.prefabs.collision import CollisionParameters, Collision
from stlib.prefabs.visual import VisualParameters, Visual
from stlib.prefabs.material import Material, MaterialParameters
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
    material : MaterialParameters
    collision : Optional[CollisionParameters] = None
    visual : Optional[VisualParameters] = None
    


class Entity(Sofa.Core.BaseEntity): 

    # A simulated object
    material : Material
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

        ### Check compatilibility of Material
        if self.parameters.material.stateType != self.parameters.template:
            print("WARNING: imcompatibility between templates of both the entity and the material")
            self.parameters.material.stateType = self.parameters.template

        self.material = self.add(Material,self.parameters.material)
        
        if self.parameters.collision is not None:
            self.collision = self.add(Collision,self.parameters.collision)
            self.addMapping(self.parameters.collision, self.collision)

        
        if self.parameters.visual is not None:
            self.visual = self.add(Visual,self.parameters.visual)
            self.addMapping(self.parameters.visual, self.visual)


    def addMapping(self, destParameter, destPrefab):

        templateString = f'{self.parameters.template},{destParameter.template}'
        
        if( self.parameters.template == StateType.VEC3):
            if isinstance(destParameter.geometry,ExtractParameters):
                destPrefab.addObject("IdentityMapping", input="@../material/", output="@.", template=templateString)
            else :
                destPrefab.addObject("BarycentricMapping", input="@../material/", output="@.", template=templateString)
        else:
            destPrefab.addObject("RigidMapping", input="@../material", output="@.", template=templateString)
