from stlib.core.parameters import EntityParameters
import dataclasses
from typing import Callable, Optional, overload
import Sofa



class Entity(Sofa.Core.BasePrefab): 

    # A simulated object
    simulation : Simulation
    visual : VisualModel
    collision : CollisionModel
    
    parameters : EntityParameters
    
    @dataclasses.dataclass
    class Parameters(EntityParameters): 
        name : str = "Entity"

        addSimulation : Callable = Simulation
        addCollisionModel : Callable = CollisionModel
        addVisualModel : Callable = VisualModel 

        #setConstitutiveLaw # : Callable = addBidule
        #setBoundaryCondition #: Callable = addBidule
        
        mechanical : dict = dataclasses.field(default_factory=dict)
        collision : CollisionModel.Parameters = CollisionModel.Parameters()
        visual : VisualModelParameters = VisualModelParameters()
        simulation : SimulationParameters = SimulationParameters()
        

    def __init__(self, parent=None, parameters=EntityParameters(), **kwargs):
        Sofa.Core.Node.__init__(self, name=parameters.name)        

        if parent is not None: 
            parent.addChild(self)
        
        self.parameters = parameters

        self.addMechanicalModel(**parameters.mechanical)
        self.addSimulation(parameters=parameters.simulation)
        self.addVisualModel(parameters=parameters.visual)
        self.addCollisionModel()

    def addMechanicalModel(self, **kwargs): 
        self.addObject("MechanicalObject", **kwargs)

    def addSimulation(self, **kwargs): 
        self.parameters.addSimulation(self, **kwargs)

    def addVisualModel(self, **kwargs):
        self.parameters.addVisualModel(self, **kwargs)

    def addCollisionModel(self):
        pass 
