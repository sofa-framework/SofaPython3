from stlib.core.basePrefab import BasePrefab
from Sofa.Core import Node

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



class NodeModifier(Sofa.Core.Controller):

    parameters : BaseNodeModifierParameters

    def __init__(self, parameters : BaseNodeModifierParameters):
        Sofa.Core.Controller.__init__(self, **(parameters.toDict()))
        self.parameters = parameters



class SingleNodeModifier(NodeModifier):

    def __init__(self, parameters : BaseNodeModifierParameters):
        super().__init__(parameters)

    def modify(self, node : Node, holder : Node = None):
        self.parameters.modifier(node)
        self.register(holder, node)



class PairNodeModifier(NodeModifier):

    def __init__(self, parameters : BaseNodeModifierParameters):
        super().__init__(parameters)

    def modify(self, node1 : Node, node2 : Node, holder : Node = None):
        self.parameters.modifier(node1,node2)
        self.register(holder, node1, node2)



class MultiNodeModifier(NodeModifier):

    def __init__(self, parameters : BaseNodeModifierParameters):
        super().__init__(parameters)

    def modify(self, *nodes : Node , holder : Node = None):
        self.parameters.modifier(*nodes)
        self.register(holder, *nodes)




@dataclasses.dataclass
class FixConstraintParameters(SingleNodeModifierParameters):
    constraintType : ConstraintType = ConstraintType.PROJECTIVE
    boxROI : list[ list[ float ] ] = dataclasses.field(default_factory= [[0,0,0], [1,1,1]])

    def modifier(self, node : Node):
        splib.mechanics.fix_points.addFixation(node.Material, type = self.constraintType, boxROIs = self.boxROI)
        pass



@dataclasses.dataclass
class AttachmentConstraintParameters(PairNodeModifierParameters):
    constraintType : ConstraintType = ConstraintType.PROJECTIVE

    def modifier(self, node1 : Node, node2 : Node):
        # splib.mechanics.fix_points.addFixation(node, type = self.constraintType, boxROI = self.boxROI)
        pass



@dataclasses.dataclass
class SimulationSolversParameters(SingleNodeModifierParameters):
    constraintType : ConstraintType = ConstraintType.PROJECTIVE
    boxROI : list[ list[ float ] ] = DEFAULT_VALUE

    def modifier(self, node1 : Node):
        splib.simulation.ode_solvers.addImplicitODE(node1)
        splib.simulation.linear_solvers.addLinearSolver(node1, constantSparsity=False, )
        pass



@dataclasses.dataclass
class SimulationSettingsParameters(SingleNodeModifierParameters):
    useLagrangian : bool = False

    def modifier(self, node1 : Node):
        if(self.useLagrangian):
            splib.simulation.headers.setupLagrangianCollision(root, displayFlags = "showVisualModels",backgroundColor=[0.8, 0.8, 0.8, 1],
                                                              parallelComputing = True,alarmDistance=0.3, contactDistance=0.02,
                                                              frictionCoef=0.5, tolerance=1.0e-4, maxIterations=20)
        else:
            splib.simulation.headers.setupDefaultHeader(root, displayFlags = "showVisualModels",
                                                        backgroundColor=[0.8, 0.8, 0.8, 1],
                                                        parallelComputing = True)