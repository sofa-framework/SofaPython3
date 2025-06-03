from stlib.entities import Entity, EntityParameters
from stlib.entities.deformable.__parameters__ import  DeformableParameters
from stlib.prefabs.visual import Visual
from stlib.mixins.collision import CollisionMixin
from splib.core.enum_types import ConstitutiveLaw

class DeformableParameters(EntityParameters):

    constitutiveLaw : ConstitutiveLaw = ConstitutiveLaw.ELASTIC

class Deformable(CollisionMixin, Entity):

    params : DeformableParameters

    @staticmethod
    def getParameters(**kwargs) -> DeformableParameters:
        return DeformableParameters(**kwargs)
        

    def __init__(self, params : DeformableParameters, **kwargs):
        Entity.__init__(self, **kwargs)  
        
        self.__addConstitutiveLaw__()
        self.addCollision(params.collision)


    #@customizable
    # Need generic way of defining paramaters (linear/hyper...)
    def __addConstitutiveLaw__(self):
        self.params.addConstitutiveLaw()

    
    #@customizable
    def __addVisual__(self):
        #Extract surface and add identity mapping
        self.add(Visual, self.params.visual)

