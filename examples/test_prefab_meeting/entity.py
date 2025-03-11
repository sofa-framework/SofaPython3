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

class Entity(Sofa.Core.Node): 
    # A simulated object
    visual : VisualModel
    collision : CollisionModel

    @staticmethod
    def addSolver(self):
        pass

    @dataclasses.dataclass
    class Parameters(object): 
        addSolver : Callable = addBidule
        addConstitutiveLaw : Callable = addBidule
        addBoundaryCondition : Callable = addBidule

        mechanical : MechanicalObject.Parameters = MechanicalObject.Parameters()
        collision : CollisionModel.Parameters = CollisionModel.Parameters()
        visual : VisualModel.Parameters = VisualModel.Parameters()

        name = "Entity"

        def to_dict(self):
            return dataclasses.asdict(self)

    # Ici mettre un prefab simplfié. 
    def __init__(self, **kwargs):
        print("CCOUou")
        params = kwargs.get("params", None) #  type: Entity.Parameters
        Sofa.Core.Node.__init__(self, params.name)

        print("ZUT ZUT ")
    
        print("COUCOU")
        self.addObject("MechanicalObject", **params.mechanical.to_dict())
        print("COUCO S")

        #if(params.visual != None):
        #    self.addChild(VisualModel(params.visual), 
        #                  name="visual")

        #if(params.collision != None):
        #    self.addChild(CollisionModel(params.collision), 
        #                  name="collision")

class Rigid(Entity):
    def __init__(self, **kwargs):
        Entity.__init__(self, **kwargs)    

    class Parameters(Entity.Parameters): 
        pass

class Deformable(Entity):
    def __init__(self, **kwargs):
        print("ZUT")
        Entity.__init__(self, **kwargs)    
        print("DEFORMABLE")

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

