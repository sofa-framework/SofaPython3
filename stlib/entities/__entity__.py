from stlib.core.baseParameters import BaseParameters
from stlib.prefabs.collision import CollisionParameters, Collision
from stlib.prefabs.visual import VisualParameters, Visual
from stlib.prefabs.behavior import Behavior, BehaviorParameters
from stlib.geometry import Geometry
import dataclasses
from typing import Callable, Optional, overload, Any
from stlib.geometry import GeometryParameters
import Sofa


@dataclasses.dataclass
class EntityParameters(BaseParameters): 
    name = "Entity"

    # addSimulation : Callable = Simulation
    #setConstitutiveLaw # : Callable = addBidule
    #setBoundaryCondition #: Callable = addBidule

    addCollision : Optional[Callable] = lambda x : Collision(CollisionParameters())
    addVisual : Optional[Callable] = lambda x : Visual(VisualParameters()) 

    geometry : GeometryParameters
    # mechanical : dict = dataclasses.field(default_factory=dict)
    collision : Optional[CollisionParameters] = None
    visual : Optional[VisualParameters] = None
    # simulation : SimulationParameters = SimulationParameters()
    


class Entity(Sofa.Core.BaseEntity): 

    # A simulated object
    behavior : Behavior
    visual : Visual
    collision : Collision
    geometry : Geometry
    
    parameters : EntityParameters
    

    def __init__(self, parent=None, parameters=EntityParameters(), **kwargs):
        Sofa.Core.Node.__init__(self, name=parameters.name)        

        if parent is not None: 
            parent.addChild(self)
        
        self.parameters = parameters

        self.add(Geometry, self.parameters.geometry)
        self.addBehavior(parameters=parameters.behavior)
        self.addVisual(parameters=parameters.visual)
        self.addCollision(parameters=parameters.collision)

    def addBehavior(self, **kwargs): 
        self.parameters.addBehavior(self, **kwargs)

    def addVisual(self, **kwargs):
        self.parameters.addVisual(self, **kwargs)

    def addCollision(self, **kwargs):
        self.parameters.addCollision(self, **kwargs)
