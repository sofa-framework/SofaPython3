from numpy.f2py.auxfuncs import throw_error

import Sofa
from Sofa.Core import Node

from typing import final
from warnings import warn
import dataclasses


@dataclasses.dataclass
class BaseNodeModifierParameters(object):
    name : str = "NodeModifier"
    kwargs : dict = dataclasses.field(default_factory=dict)

    _numberOfAffectedNodes : int = -1

    def doModify(self, owner, node : list[Node]):
        pass

    @final
    def _modifier(self, owner, node : list[Node]):
        if self._numberOfAffectedNodes == -1:
            warn(f"Parameter type {type(self)} doesn't set the attribute __numberOfAffectedNodes. No check on the number of nodes given as input will be made.")
        elif len(node) != self._numberOfAffectedNodes:
            raise AttributeError(f"This modifier required {self._numberOfAffectedNodes} nodes to be applied.")
        return self.doModify(owner, node)

    def toDict(self):
        return dataclasses.asdict(self)





#TODO use Component when available
class NodeModifier(Sofa.Core.Controller):

    parameters : BaseNodeModifierParameters

    def __init__(self, parameters : BaseNodeModifierParameters):
        Sofa.Core.Controller.__init__(self, **(parameters.toDict()))
        self.parameters = parameters

    def apply(self, owner,  nodes : list[Node] ):
        self.parameters._modifier(owner,  nodes)

