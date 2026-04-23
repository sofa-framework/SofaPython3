from stlib.entities import Entity, EntityParameters
from stlib.visual import VisualParameters
from stlib.geometries.file import FileParameters
from stlib.materials.deformable import DeformableMaterialParameters
from splib.core.enum_types import ElementType

from typing import Optional


class SphereParameters(EntityParameters):
    name : str = "Sphere"
    radius : float = 1
    deformable : bool = False
    visual : Optional[VisualParameters] = VisualParameters(geometry = FileParameters(filename="mesh/sphere.obj"))
    
    def model_post_init(self, _):
        # TODO:
        # 1. apply size as scale in geometry, material, collision and visual 
        if self.deformable:
            self.geometry = FileParameters(filename="mesh/sphere.vtk", elementType=ElementType.TETRAHEDRA)
            self.material = DeformableMaterialParameters()
        return  


class Sphere(Entity):

    def __init__(self, parameters: SphereParameters):
        super().__init__(parameters)