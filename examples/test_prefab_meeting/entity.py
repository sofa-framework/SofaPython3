from typing import Callable, Optional, overload

import Sofa
import dataclasses


class VisualModel(Sofa.Core.BasePrefab):
    def __init__(self, params, **kwargs):
        Sofa.Core.Node.__init__(self, **kwargs)

    class Parameters(object):
        enabled : bool  
        filename : str

    class DefaultParameters(object):
        enabled : bool  
        filename : str

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

        
def addBidule(self):
    return self.addChild("Bidule") 


class Entity(Sofa.Core.Prefab):
    def __init__(self, **kwargs):
        self.addChild(VisualModel(**kwargs), name="visual")
        self.addChild(CollisionModel(**kwargs), name="visual")

    def addConstitutiveLaw(self):
        pass 

    def addSolver(self):
        pass 

DEFAULT_VALUE = object()

def addSolver(node : Sofa.Core.Node, template=DEFAULT_VALUE, numiterations=DEFAULT_VALUE, **kwargs):
    node.addObject("ODESolver", name = "integrationscheme",numiterations = numiterations, **kwargs)
    node.addObject("LinearSolver", name = "numericalsolver", template=DEFAULT_VALUE, **kwargs)

#@dataclasses.dataclass
#class Solver(object):
#    integrationscheme : str
#    numericalsolver : str

@dataclasses.dataclass
class SolverParameters(object):
    numiteration : int | object = DEFAULT_VALUE
    template: str | object = DEFAULT_VALUE

    kwargs : dict = {"intergrato"} 

    def to_dict(self):
        return self.asdict() | self.kwargs

class Entity(Sofa.Core.Prefab): 
    # A simulated object
    visual : VisualModel
    collision : CollisionModel

    parameters : EntityParameters

    addSolver : PrefabMethod 

    def __init__(self, params):        
        self.params = params 
        
        self.addChild(VisualModel(params.visual), name="visual")

        self.addSolver()
        self.addCollisionModel()

    #@staticmethod
    #def addSolver(self): 
    #    pass 

    #def defaultAddSolver(self):
    #    pass

    @prefab.method
    def addCollisionModel(self):
        pass 

@dataclasses.dataclass
class EntityParameters(object): 
        addSolver : Callable = addSolver
        setConstitutiveLaw # : Callable = addBidule
        setBoundaryCondition #: Callable = addBidule
        
        addCollisionModel : CollisionModel

        #mechanical : MechanicalObject.Parameters = MechanicalObject.Parameters()
        collision : CollisionModel.Parameters = CollisionModel.Parameters()
        visual : VisualModel.Parameters = VisualModel.Parameters()
        solver : SolverParameters = SolverParameters()
        
        kwargs : dict = {} 
        def to_dict(self):
            return dataclasses.asdict(self)


class Rigid(Entity):
    def __init__(self, **kwargs):
        Entity.__init__(self, **kwargs)    

    class Parameters(Entity.Parameters): 
        pass

class Deformable(Entity):
    def __init__(self, **kwargs):
        Entity.__init__(self, **kwargs)    
        
    class Parameters(Entity.Parameters): 
        addConstitutiveLaw : Callable
        mass = 3.4
        name = "Deformable"

        def to_dict(self):
            return dataclasses.asdict(self)


Entity.Rigid = Rigid
Entity.Deformable = Deformable

class SoftRobots:
    class Cable(Sofa.Core.BasePrefab):
        length : float 

        def __init__(self,**kwargs):
            pass

        def Parameters(object):
            lenght : float    

class Trunk(Sofa.Core.BasePrefab):
    body : Entity.Deformable
    cables : list [SoftRobots.Cable]
    
    def __init__(self, params):
        body = Entity.Deformable()

        for param in range(params.cables):
            cables.append(SoftRobots.Cable(body, param))

    class Parameters(object):
        body : Entity.Deformable.Parameters
        cables : list[SoftRobots.Cable.Parameters]

