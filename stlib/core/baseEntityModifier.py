from stlib.core.basePrefab import BasePrefab
import Sofa.Core


class BaseEntityModifier(BasePrefab):
    """
    An EntityModifier is a Prefab that modifies a set of Entities
    """

    nodeName = "Modifiers"
    creator : Sofa.Core.Node = None

    def __init__(self, parameters):
        BasePrefab.__init__(self, parameters)

    def apply(self, **kwargs):
        if not self.creator.getChild(self.nodeName):
            self.creator.modifiers = self.creator.add(Sofa.Core.Node(self.nodeName))

        self.creator.modifiers.add(self)
        self._apply(**kwargs)

    def _apply(self, **kwargs):
        raise NotImplemented("To be overridden by child class")