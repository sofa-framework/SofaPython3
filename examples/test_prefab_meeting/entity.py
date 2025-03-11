from typing import Callable, Optional, overload

import Sofa
import dataclasses
                
def addBidule(self):
    return self.addChild("Bidule") 

DEFAULT_VALUE = object()

def NONE(*args, **kwargs):
    pass

def to_dict(o):
    if isinstance(o, dict):
        return o
    if hasattr(o, "to_dict"):
        return o.to_dict()
    return {}

@dataclasses.dataclass
class PrefabParameters(object): 
    name : str = "Prefab"
    kwargs : dict = dataclasses.field(default_factory=dict)

    def __getattr__(self, name: str) :
        if name == "__getstate__":
            getattr(PrefabParameters, "__getstate__")
        if name == "__setstate__":
            getattr(PrefabParameters, "__setstate__")

        try: 
            a =  self.__getattribute__(name)
        except Exception as e: 
            return NONE
        return a

    def to_dict(self):
        return dataclasses.asdict(self)

@dataclasses.dataclass
class VisualModelParameters(PrefabParameters):
    name : str = "VisualModel"

    filename : str = "mesh/sphere_02.obj"
    
    renderer : dict = dataclasses.field(default_factory=dict)
    mapping : dict = dataclasses.field(default_factory=dict)

class VisualModel(Sofa.Core.Node):
     
    def __init__(self, parent=None, parameters : VisualModelParameters = VisualModelParameters()):
        Sofa.Core.Node.__init__(self, name=parameters.name)

        if parent != None:
            parent.addChild(self)

        self.addObject("MeshOBJLoader", name="loader", filename=parameters.filename)
        self.addRenderer(**to_dict(parameters.renderer) | {"src" : "@loader"} )
        self.addMapping(**to_dict(parameters.mapping) )

    def addRenderer(self, **kwargs):
        self.addObject("OglModel", name="renderer", **kwargs)

    def addMapping(self, **kwargs):
        self.addObject("RigidMapping", name="mapping", **kwargs)
        
class CollisionModel(Sofa.Core.BasePrefab):
    def __init__(self, params, **kwargs):
        Sofa.Core.Node.__init__(self, **kwargs)

    class Parameters(object):
        enabled : bool = False 

class MechanicalObject(Sofa.Core.Object):
    positions : list[float]

    @dataclasses.dataclass
    class Parameters(object):
        name : str = "MechanicalObject"

        def to_dict(self):
            return dataclasses.asdict(self)


@dataclasses.dataclass
class SimulationParameters(PrefabParameters):
    name : str = "Simulation"
    iterations : Optional[int] = None 
    template: Optional[str] = None 
    solver : dict = dataclasses.field(default_factory=dict)
    integration : dict = dataclasses.field(default_factory=dict)

    def to_dict(self):
        return self.asdict() 

class Simulation(Sofa.Core.Node):
    solver : Sofa.Core.Object 
    integration : Sofa.Core.Object
    iterations : int

    def __init__(self, parent : Sofa.Core.Node = None, parameters : SimulationParameters = SimulationParameters()):
        Sofa.Core.Node.__init__(self, name=parameters.name)
        if parent is not None:
            parent.addChild(self)
    
        if parameters.iterations != NONE and "iterations" in parameters.solver:
            raise Exception("Cannot set direct attribute and internal hack... ")

        self.addObject("EulerImplicitSolver", name = "integration", **to_dict(parameters.integration))
        self.addObject("CGLinearSolver", name = "solver", iterations=parameters.iterations, **to_dict(parameters.solver))
        


#@dataclasses.dataclass
#class Solver(object):
#    integrationscheme : str
#    numericalsolver : str

@dataclasses.dataclass
class EntityParameters(PrefabParameters): 
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
        
class Entity(Sofa.Core.Node): 
    # A simulated object
    simulation : Simulation
    visual : VisualModel
    collision : CollisionModel
    
    parameters : EntityParameters

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

class Rigid(Entity):
    def __init__(self, **kwargs):
        Entity.__init__(self, **kwargs)    


class Deformable(Entity):
    def __init__(self, **kwargs):
        Entity.__init__(self, **kwargs)    

@dataclasses.dataclass
class DeformableEntityParameters(EntityParameters):     
    addConstitutiveLaw : Callable = lambda x: x

    mass : Optional[float] = None    

    def to_dict(self):
        return dataclasses.asdict(self)



