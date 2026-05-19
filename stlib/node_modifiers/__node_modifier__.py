from numpy.f2py.auxfuncs import throw_error

import Sofa
from Sofa.Core import Node

from typing import final
from warnings import warn
import dataclasses

from functools import wraps

def AffectedNodes(nbOfNodes):
    def _AffectedNodes(method):
        @wraps(method)
        def wrapper(*args, **kwargs):

            if(len(args) < 3):
                raise ValueError("No list of node was given as argument to the node modifier")

            if isinstance(args[2], list) and (len(args[2]) != nbOfNodes):
                raise ValueError(f"Number of node to modify mismatch : {nbOfNodes} where expected, only {len(args[1])} where given.\n"
                             "The affected nodes are passed through the call to add when adding the modifier to the graph as the keywork argument \"on\" and must be in a form o list.")
            return method(*args, **kwargs)
        return wrapper
    return _AffectedNodes

@dataclasses.dataclass
class BaseNodeModifierParameters(object):
    name : str = "NodeModifier"
    kwargs : dict = dataclasses.field(default_factory=dict)

    @AffectedNodes(0)
    def modify(self, owner, node : list[Node]) -> list[Node] :
        return []


    def toDict(self):
        return dataclasses.asdict(self)





#TODO use Component when available
class NodeModifier(Sofa.Core.Component):

    parameters : BaseNodeModifierParameters

    def __init__(self, parameters : BaseNodeModifierParameters):
        Sofa.Core.Component.__init__(self, **(parameters.toDict()))
        self.parameters = parameters

    def register( self, owner, nodes : list[Node]) :
        if len(nodes) == 0:
            raise ValueError(f"No node seems to have been modified by the node modifier {self.parameters.name}. Make sure the \"modify\" method of the used parameter type returns the list of nodes modified by this modifier.")

        #Remove last / if exists (this is only the case for root
        holderPath = str(owner.getLinkPath())
        if holderPath[-1] == "/":
            holderPath = holderPath[:-1]

        #Find Relative paths to nodes on which it is applied + add myself to data
        for node in nodes:
            targetPath = str(node.getLinkPath())
            relPath = targetPath.removeprefix(holderPath)
            relPath =  "@" + relPath.count('/')*"../" + "Modifiers/" + self.name.value


            if node.getData("modifiedBy") is None:
                node.addData("modifiedBy", type="vector<string>",value=[], default=[], help="Modifiers that modified this Node", group="STLIB")

            node.modifiedBy = node.modifiedBy.value + [relPath]

        #Add targets to self data
        for node in nodes:
            targetPath = str(node.getLinkPath())
            relPath = targetPath.removeprefix(holderPath)
            relPath = "@.." + relPath

            if self.getData("modified") is None:
                self.addData("modified", type="vector<string>",value=[], default=[], help="Nodes modified by this modifier", group="STLIB")

            self.modified = self.modified.value + [relPath]


    def apply(self, owner,  nodes : list[Node] ) -> list[Node]:
        return self.parameters.modify(owner,  nodes)

