from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters, Optional, dataclasses
from stlib.geometries import Geometry, GeometryParameters
from stlib.geometries.file import FileParameters
from splib.core.enum_types import CollisionPrimitive
from splib.core.utils import DEFAULT_VALUE
from splib.mechanics.collision_model import addCollisionModels
from Sofa.Core import Object 

@dataclasses.dataclass
class CollisionParameters(BaseParameters):
    name : str = "Collision"
    
    primitives : list[CollisionPrimitive] = dataclasses.field(default_factory = lambda :[CollisionPrimitive.TRIANGLES])

    selfCollision : Optional[bool] = DEFAULT_VALUE
    bothSide : Optional[bool] = DEFAULT_VALUE
    group : Optional[int] = DEFAULT_VALUE
    contactDistance : Optional[float] = DEFAULT_VALUE

    geometry : GeometryParameters = dataclasses.field(default_factory = lambda : GeometryParameters())


class Collision(BasePrefab):
    def __init__(self, parameters: CollisionParameters):
        BasePrefab.__init__(self, parameters)

    def init(self):

        geom = self.add(Geometry, parameters = self.parameters.geometry)
        
        self.addObject("MechanicalObject", template="Vec3", position=f"@{self.parameters.geometry.name}/container.position")
        for primitive in self.parameters.primitives:
            addCollisionModels(self, primitive,
                               topology=f"@{self.parameters.geometry.name}/container",
                               selfCollision=self.parameters.selfCollision, 
                               group=self.parameters.group, 
                               **self.parameters.kwargs)
            

    @staticmethod
    def getParameters(**kwargs) -> CollisionParameters:
        return CollisionParameters(**kwargs)


def createScene(root):

    root.addObject("VisualStyle", displayFlags="showCollisionModels")

    # Create a visual from a mesh file
    parameters = Collision.getParameters()
    parameters.group = 1
    parameters.geometry = FileParameters(filename="mesh/cube.obj")
    # Expert parameters
    # parameters.kwargs = {
    #                     "TriangleCollisionModel":{"contactStiffness": 100.0, "contactFriction": 0.5}
    #                 }
    collision = root.add(Collision, parameters)

    # OR set the parameters post creation
    # collision.TriangleCollisionModel.contactStiffness = 100.0
    # collision.TriangleCollisionModel.contactFriction = 0.5
    # collision.TriangleCollisionModel.set(contactStiffness=100.0, contactFriction=0.5) # we have information of what is possible
    # collision.TriangleCollisionModel.set({"contactStiffness": 100.0, "contactFriction": 0.5}) # we can do n'importe quoi 
