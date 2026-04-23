from stlib.entities import Entity, EntityParameters
from stlib.materials.deformable import DeformableMaterialParameters
from stlib.geometries.file import FileParameters
from splib.core.enum_types import ElementType


class CubeParameters(EntityParameters):
    name : str = "Cube"
    size : float = 1
    deformable : bool = False

    def model_post_init(self, _):
        # TODO : 
        # 1. apply size as scale in geometry, material, collision and visual 
        if self.deformable:
            self.geometry = FileParameters(filename="mesh/sphere.vtk", elementType=ElementType.TETRAHEDRA)
            self.material = DeformableMaterialParameters()
        return 


class Cube(Entity):

    def __init__(self, parameters: CubeParameters):
        super().__init__(parameters)