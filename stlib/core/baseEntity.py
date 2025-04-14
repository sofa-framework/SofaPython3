import Sofa.Core
from base_parameters import BaseParameters

class BaseEntity(Sofa.Core.Prefab):

    parameters : BaseParameters

    def __init__(self):
        Sofa.Core.Prefab.__init__(self)
