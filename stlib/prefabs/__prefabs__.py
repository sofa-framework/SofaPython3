from stlib.core.baseParameters import BaseParameters
from stlib.core.basePrefab import BasePrefab


class PrefabParameters(BaseParameters): 
    """
    Attributes:
        name (str): The name of the entity.
        addCollision (Optional[Callable]): Optional callable to add a collision component to the entity.
        addVisual (Optional[Callable]): Optional callable to add a visual component to the entity.
        geometry (GeometryParameters): Parameters for the geometry of the entity, with a default of a point at the origin.
        material (MaterialParameters): Parameters for the material of the entity, with a default of a rigid material.
        visual (Optional[VisualParameters]): Optional parameters for the visual component of the entity, with a default of a cube mesh.
        collision (Optional[CollisionParameters]): Optional parameters for the collision component of the entity, with a default of None.
    """
    name : str = "Prefab"
    

class Prefab(BasePrefab): 
    """
    A Prefab is a Sofa.Node that assembles a set of components and nodes

    Attributes:
        parameters (PrefabParameters): The parameters defining the prefab, including its name and any other relevant properties. 
                                       This is a dataclass that should be defined in the child class, inheriting from BasePrefabParameters, 
                                       and can include any additional parameters needed for the specific prefab.
    """
    parameters : PrefabParameters
    
    def __init__(self, parameters: PrefabParameters):
        BasePrefab.__init__(self, parameters)
