import Sofa.Core
from baseParameters import BaseParameters

class BaseEntity(Sofa.Core.Prefab):

    parameters : BaseParameters

    def __init__(self):
        Sofa.Core.Prefab.__init__(self)
