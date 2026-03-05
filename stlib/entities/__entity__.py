from stlib.core.baseParameters import BaseParameters
from splib.core.enum_types import StateType
from stlib.core.basePrefab import BasePrefab

from stlib.geometries import Geometry
from stlib.geometries import GeometryParameters, InternalDataProvider
from stlib.geometries.file import FileParameters
from stlib.materials.rigid import RigidParameters
from stlib.materials import Material, MaterialParameters
from stlib.visual import VisualParameters, Visual
from stlib.collision import CollisionParameters, Collision
from splib.core.enum_types import ElementType

import dataclasses
from typing import Optional


@dataclasses.dataclass
class EntityParameters(BaseParameters): 
    name : str = "Entity"

    stateType : StateType = StateType.RIGID

    geometry : GeometryParameters = GeometryParameters(elementType = ElementType.POINTS, data = InternalDataProvider(position = [[0., 0., 0.]]))
    material : MaterialParameters = RigidParameters()
    visual : Optional[VisualParameters] = VisualParameters(geometry = FileParameters(filename="mesh/cube.obj"))
    collision : Optional[CollisionParameters] = None
            

class Entity(BasePrefab): 

    material : Material
    visual : Visual
    collision : Collision
    geometry : Geometry
    
    parameters : EntityParameters
    

    def __init__(self, parameters : EntityParameters):
        BasePrefab.__init__(self, parameters)


    def init(self):
        self.geometry = self.add(Geometry, parameters=self.parameters.geometry)
        self.checkMaterialCompatibility

        self.material = self.add(Material, parameters=self.parameters.material)
        self.material.getMechanicalState().position.parent = self.geometry.container.position.linkpath
        
        if self.parameters.collision is not None:
            self.collision = self.add(Collision, parameters=self.parameters.collision)
            self.addMapping(self.collision)
        
        if self.parameters.visual is not None:
            self.visual = self.add(Visual, parameters=self.parameters.visual)
            self.addMapping(self.visual)


    def checkMaterialCompatibility(self):
        if self.parameters.material.stateType != self.parameters.stateType:
            print("WARNING: imcompatibility between templates of both the entity and the material")
            self.parameters.material.stateType = self.parameters.stateType


    def addMapping(self, destinationPrefab: BasePrefab):

        template = f'{self.parameters.stateType},Vec3' # TODO: check that it is always true

        #TODO: all paths are expecting Geometry to be called Geomtry and so on. 
        # We need to robustify this by using the name parameter somehow
        if( self.parameters.stateType == StateType.VEC3):
            destinationPrefab.addObject("BarycentricMapping", 
                                        output=destinationPrefab.linkpath, 
                                        output_topology=destinationPrefab.Geometry.container.linkpath,
                                        input=self.material.linkpath,
                                        input_topology=self.geometry.container.linkpath,
                                        template=template)
        else:
            destinationPrefab.addObject("RigidMapping", 
                                         output=destinationPrefab.linkpath, 
                                        input=self.material.linkpath,
                                        template=template)
