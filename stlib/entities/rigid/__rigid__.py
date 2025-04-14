from stlib.core.entity import Entity
from stlib.core.parameters import EntityParameters

import dataclasses


class Rigid(Entity):
    def __init__(self, **kwargs):
        Entity.__init__(self, **kwargs)    
