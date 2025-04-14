import Sofa.Core
from baseParameters import BaseParameters


class BasePrefab(Sofa.Core.Prefab):
    """
    A Prefab is a Sofa.Node that assembles a set of components and nodes
    """

    def __init__(self, params: BaseParameters):
        Sofa.Core.Prefab.__init__(self, name=params.name)