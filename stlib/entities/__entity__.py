from stlib.core.baseParameters import BaseParameters
from stlib.core.basePrefab import BasePrefab
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
    name : str = "Entity"

    stateType : StateType = StateType.VEC3

    ### QUID 
    addCollision : Optional[Callable] = lambda x : Collision(CollisionParameters())
    addVisual : Optional[Callable] = lambda x : Visual(VisualParameters()) 

    geometry : GeometryParameters = None
    material : MaterialParameters = None
    collision : Optional[CollisionParameters] = None
    visual : Optional[VisualParameters] = None
    


class Entity(BasePrefab): 

    # A simulated object
    material : Material
    visual : Visual
    collision : Collision
    geometry : Geometry
    
    parameters : EntityParameters
    

    def __init__(self, parameters=EntityParameters(), **kwargs):
        BasePrefab.__init__(self, parameters)

        self.parameters = parameters

        self.geometry = self.add(Geometry, self.parameters.geometry)

        ### Check compatilibility of Material
        if self.parameters.material.stateType != self.parameters.stateType:
            print("WARNING: imcompatibility between templates of both the entity and the material")
            self.parameters.material.stateType = self.parameters.stateType

        self.material = self.add(Material, self.parameters.material)
        self.material.States.position.parent = self.geometry.container.position.linkpath
        
        if self.parameters.collision is not None:
            self.collision = self.add(Collision, self.parameters.collision)
            self.addMapping(self.collision)
        
        if self.parameters.visual is not None:
            self.visual = self.add(Visual, self.parameters.visual)
            self.addMapping(self.visual)


    def addMapping(self, destinationPrefab):

        template = f'{self.parameters.stateType},Vec3' # TODO: check that it is always true
        
        if( self.parameters.stateType == StateType.VEC3):
            destinationPrefab.addObject("BarycentricMapping", 
                                        input="@../Material/", 
                                        input_topology=destinationPrefab.geometry.container.linkpath,
                                        output="@.", 
                                        template=template)
        else:
            destinationPrefab.addObject("RigidMapping", 
                                        input="@../Material", 
                                        output="@.", 
                                        template=template)
