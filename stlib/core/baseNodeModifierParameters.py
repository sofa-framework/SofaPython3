import dataclasses
from splib.core.utils import DEFAULT_VALUE

from Sofa.Core import Node
from typing import Callable, Optional, Any

@dataclasses.dataclass
class BaseNodeModifierParameters(object):
    name : str = "NodeModifier"
    kwargs : dict = dataclasses.field(default_factory=dict)

    def modifier(self, *node : Node):
        pass
    def toDict(self):
        return dataclasses.asdict(self)





@dataclasses.dataclass
class SingleNodeModifierParameters(BaseNodeModifierParameters):
    def modifier(self, node : Node):
        pass



@dataclasses.dataclass
class PairNodeModifierParameters(BaseNodeModifierParameters):
    def modifier(self, node1 : Node, node2 : Node):
        pass


