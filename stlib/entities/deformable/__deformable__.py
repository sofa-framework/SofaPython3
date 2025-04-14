from stlib.entities import Entity
from stlib.entities.deformable.__parameters__ import  DeformableParameters
from stlib.geometry import Geometry
from stlib.prefabs.visual import Visual
from stlib.prefabs.collision import Collision

class Deformable(Entity):
    params : DeformableParameters

    @staticmethod
    def getParameters(**kwargs) -> DeformableParameters:
        return DeformableParameters(**kwargs)
        

    def __init__(self, params : DeformableParameters, **kwargs):
        Entity.__init__(self, **kwargs)   
        self.add(Geometry, params.geometry)
        self.__addConstitutiveLaw__()
        self.add(Collision, params.collision)

    #@customizable
    # Need generic way of defining paramaters (linear/hyper...)
    def __addConstitutiveLaw__(self):
        self.params.addConstitutiveLaw()

    
    
    #@customizable
    def __addVisual__(self):
        #Exctract surface and add indentity mapping

        self.add(Visual, params.visual)

        pass
