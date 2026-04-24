import dataclasses
import splib
from stlib.node_modifiers import BaseNodeModifierParameters, AffectedNodes
from splib.core.enum_types import ConstraintType
from splib.core.utils import DEFAULT_VALUE

from typing import override
from Sofa.Core import Node

@dataclasses.dataclass
class FixConstraintParameters(BaseNodeModifierParameters):
    constraintType : ConstraintType = ConstraintType.PROJECTIVE
    boxROIs : list[ list[ float ] ] = DEFAULT_VALUE
    sphereROIs : list[ list[ float ] ] = DEFAULT_VALUE
    indices : list[ int ] = DEFAULT_VALUE
    fixAll : bool = DEFAULT_VALUE

    @override
    @AffectedNodes(1)
    def modify(self, owner, nodes : list[Node]) -> list[Node]:
        splib.mechanics.attachment.addFixation(nodes[0].Material, type = self.constraintType, boxROIs = self.boxROIs, sphereROIs=self.sphereROIs, indices=self.indices, fixAll=self.fixAll)
        return nodes



@dataclasses.dataclass
class AttachmentConstraintParameters(BaseNodeModifierParameters):
    constraintType : ConstraintType = ConstraintType.WEAK
    indices1 : list[int] =  dataclasses.field(default_factory= [0])
    indices2 : list[int] =  dataclasses.field(default_factory= [0])
    stiffness : float = DEFAULT_VALUE
    damping : float = DEFAULT_VALUE

    @override
    @AffectedNodes(2)
    def modify(self, owner, node : list[Node]) -> list[Node]:
        splib.mechanics.attachment.attachObjects(owner,  type = self.constraintType, object1=node[0], object2=node[1],  indices1=self.indices1, indices2=self.indices2, stiffness=self.stiffness, damping=self.damping, **self.kwargs)
        return node + [owner]


