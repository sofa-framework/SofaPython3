from stlib.entities import Entity
from stlib.entities.rigid.__parameters__ import  RigidParameters

import dataclasses


class Rigid(Entity):
   
    @staticmethod
    def getParameters(**kwargs) -> RigidParameters:
        return RigidParameters(**kwargs)
        

    def __init__(self, **kwargs):
        Entity.__init__(self, **kwargs)    
