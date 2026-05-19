import Sofa
from stlib.core.baseParameters import BaseParameters

class BaseEntity(Sofa.Prefab):

    parameters : BaseParameters

    def __init__(self):
        Sofa.Prefab.__init__(self)

        
