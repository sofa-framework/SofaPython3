import Sofa.Core


class BasePrefab(Sofa.Core.Prefab):
    """
    A Prefab is a Sofa.Node that assembles a set of components and nodes
    """

    def __init__(self):
        Sofa.Core.Prefab.__init__(self)