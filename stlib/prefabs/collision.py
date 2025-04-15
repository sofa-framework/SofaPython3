from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters, Callable, Optional, dataclasses, Any
from stlib.geometry import Geometry, GeometryParameters
from stlib.geometry.file import FileParameters
from splib.core.enum_types import CollisionPrimitive
from splib.core.utils import DEFAULT_VALUE, addObject
from splib.mechanics.collision_model import addCollisionModels
from Sofa.Core import Object 

@dataclasses.dataclass
class CollisionParameters(BaseParameters):
    primitives : list[CollisionPrimitive] = dataclasses.field(default_factory = lambda :[CollisionPrimitive.POINTS])

    selfCollision : Optional[bool] = DEFAULT_VALUE
    proximity : Optional[float] = DEFAULT_VALUE
    group : Optional[int] = DEFAULT_VALUE
    contactStiffness : Optional[float] = DEFAULT_VALUE
    contactFriction : Optional[float] = DEFAULT_VALUE
    spheresRadius : Optional[float] = DEFAULT_VALUE

    geometry : GeometryParameters = dataclasses.field(default_factory = lambda : GeometryParameters())
    addMapping : Optional[Callable] = None


class Collision(BasePrefab):
    def __init__(self, params: CollisionParameters):
        BasePrefab.__init__(self, params)

        geom = self.add(Geometry, params.geometry)
        
        addObject(self,"MechanicalObject",name="MechanicalObject",template="Vec3", position=f"@{params.geometry.name}/container.position", **params.kwargs)
        for primitive in params.primitives:
            addCollisionModels(self,primitive,
                               topology=f"@{params.geometry.name}/container",
                               selfCollision=params.selfCollision, 
                               proximity=params.proximity, 
                               group=params.group, 
                               contactStiffness=params.contactStiffness, 
                               contactFriction=params.contactFriction,
                               spheresRadius=params.spheresRadius, 
                               **params.kwargs)
            
        if params.addMapping is not None:
            params.addMapping(self)

    @staticmethod
    def getParameters(**kwargs) -> CollisionParameters:
        return CollisionParameters(**kwargs)


def createScene(root):

    # Create a visual from a mesh file
    params = Collision.getParameters()
    params.geometry = FileParameters(filename="mesh/cube.obj")
    root.add(Collision, params)
