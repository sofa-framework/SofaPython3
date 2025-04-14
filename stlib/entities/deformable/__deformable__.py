from stlib.entities import Entity
from stlib.entities.deformable.__parameters__ import  DeformableParameters


class Deformable(Entity):
   
    @staticmethod
    def getParameters(**kwargs) -> DeformableParameters:
        return DeformableParameters(**kwargs)
        

    def __init__(self, params : DeformableParameters, **kwargs):
        Entity.__init__(self, **kwargs)    

