from stlib.core.baseParameters import BaseParameters
from stlib.collision import CollisionParameters, Collision
from stlib.visual import VisualParameters, Visual
from stlib.materials import Material, MaterialParameters
from stlib.geometries import Geometry
from typing import Callable, Optional
from stlib.geometries import GeometryParameters, InternalDataProvider
from splib.core.enum_types import StateType
from stlib.core.basePrefab import BasePrefab

from stlib.geometries.file import FileParameters
from stlib.materials.rigid import RigidMaterialParameters
from splib.core.enum_types import ElementType

from dynapydantic import Polymorphic
import Sofa


class EntityParameters(BaseParameters): 
    name : str = "Entity"

    ### QUID 
    addCollision : Optional[Callable] = Collision(CollisionParameters())
    addVisual : Optional[Callable] = Visual(VisualParameters()) 

    geometry : Polymorphic[GeometryParameters] = GeometryParameters(elementType = ElementType.POINTS, data = InternalDataProvider(position = [[0., 0., 0.]]))
    material : Polymorphic[MaterialParameters] = RigidMaterialParameters()
    collision : Optional[CollisionParameters] = None
    visual : Optional[VisualParameters] = VisualParameters(geometry = FileParameters(filename="mesh/cube.obj"))
    


class Entity(BasePrefab): 

    # A simulated object
    material : Material
    visual : Visual
    collision : Collision
    geometry : Geometry
    
    parameters : EntityParameters
    

    def __init__(self, parameters: EntityParameters):
        BasePrefab.__init__(self, parameters)


    def init(self):
        self.geometry = self.add(Geometry, parameters=self.parameters.geometry)

        self.material = self.add(Material, parameters=self.parameters.material)
        self.material.getMechanicalState().topology = self.geometry.container.linkpath

        if self.parameters.collision is not None:
            self.collision = self.add(Collision, parameters=self.parameters.collision)
            self.addMapping(self.collision)
        
        if self.parameters.visual is not None:
            self.visual = self.add(Visual, parameters=self.parameters.visual)
            self.addMapping(self.visual)


    def addMapping(self, destinationPrefab):

        template = f'{self.parameters.material.stateType},Vec3' # TODO: check that it is always true

        #TODO: all paths are expecting Geometry to be called Geomtry and so on. We need to robustify this by using the name parameter somehow
        if( self.parameters.material.stateType == StateType.VEC3):
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
