from stlib.core.baseParameters import BaseParameters
from stlib.collision import CollisionParameters, Collision
from stlib.visual import VisualParameters, Visual
from stlib.materials import Material, MaterialParameters
from stlib.geometries import Geometry
import dataclasses
from typing import Callable, Optional
from stlib.geometries import GeometryParameters
from splib.core.enum_types import StateType
from stlib.core.basePrefab import BasePrefab


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


    def init(self):
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
                                        output=destinationPrefab.linkpath, 
                                        output_topology=destinationPrefab.geometry.container.linkpath,
                                        input=self.material.linkpath, 
                                        input_topology=self.geometry.container.linkpath, 
                                        template=template)
        else:
            destinationPrefab.addObject("RigidMapping", 
                                         output=destinationPrefab.linkpath, 
                                        input=self.material.linkpath, 
                                        template=template)
