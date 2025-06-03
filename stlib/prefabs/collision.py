from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters, Callable, Optional, dataclasses, Any
from stlib.geometry import Geometry, GeometryParameters
from stlib.geometry.file import FileParameters
from splib.core.enum_types import CollisionPrimitive
from splib.core.utils import DEFAULT_VALUE
from splib.mechanics.collision_model import addCollisionModels
from Sofa.Core import Object 

@dataclasses.dataclass
class CollisionParameters(BaseParameters):
    primitives : list[CollisionPrimitive] = dataclasses.field(default_factory = lambda :[CollisionPrimitive.TRIANGLES])

    selfCollision : Optional[bool] = DEFAULT_VALUE
    bothSide : Optional[bool] = DEFAULT_VALUE
    group : Optional[int] = DEFAULT_VALUE
    contactDistance : Optional[float] = DEFAULT_VALUE

    geometry : GeometryParameters = dataclasses.field(default_factory = lambda : GeometryParameters())
    addMapping : Optional[Callable] = None


class Collision(BasePrefab):
    def __init__(self, params: CollisionParameters):
        BasePrefab.__init__(self, params)

        geom = self.add(Geometry, params.geometry)
        
        self.addObject("MechanicalObject", template="Vec3", position=f"@{params.geometry.name}/container.position")
        for primitive in params.primitives:
            addCollisionModels(self, primitive,
                               topology=f"@{params.geometry.name}/container",
                               selfCollision=params.selfCollision, 
                               group=params.group, 
                               **params.kwargs)
            
        if params.addMapping is not None:
            params.addMapping(self)

    @staticmethod
    def getParameters(**kwargs) -> CollisionParameters:
        return CollisionParameters(**kwargs)


def createScene(root):

    root.addObject("VisualStyle", displayFlags="showCollisionModels")

    # Create a visual from a mesh file
    params = Collision.getParameters()
    params.group = 1
    params.geometry = FileParameters(filename="mesh/cube.obj")
    # Expert parameters
    # params.kwargs = {
    #                     "TriangleCollisionModel":{"contactStiffness": 100.0, "contactFriction": 0.5}
    #                 }
    collision = root.add(Collision, params)

    # OR set the parameters post creation
    # collision.TriangleCollisionModel.contactStiffness = 100.0
    # collision.TriangleCollisionModel.contactFriction = 0.5
    # collision.TriangleCollisionModel.set(contactStiffness=100.0, contactFriction=0.5) # we have information of what is possible
    # collision.TriangleCollisionModel.set({"contactStiffness": 100.0, "contactFriction": 0.5}) # we can do n'importe quoi 
