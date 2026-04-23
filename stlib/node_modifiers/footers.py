import dataclasses
from stlib.node_modifiers import BaseNodeModifierParameters
from splib.core.enum_types import ConstraintType
from splib.core.utils import DEFAULT_VALUE
from Sofa.Core import Node
from typing import override

import splib


@dataclasses.dataclass
class SimulationSolversParameters(BaseNodeModifierParameters):
    staticODE              : bool  = False
    iterative              : bool  = False
    iterations             : int   = DEFAULT_VALUE
    tolerance              : float = DEFAULT_VALUE
    threshold              : float = DEFAULT_VALUE
    template               : str   = DEFAULT_VALUE
    constantSparsity       : bool  = False
    parallelInverseProduct : bool  = True

    _numberOfAffectedNodes = 1

    @override
    def doModify(self, owner, node : list[Node]):
        splib.simulation.ode_solvers.addImplicitODE(node[0], static=self.staticODE, **self.kwargs)
        splib.simulation.linear_solvers.addLinearSolver(node[0], iterative=self.iterative,
                                                                 iterations=self.iterations,
                                                                 tolerance=self.tolerance,
                                                                 threshold=self.threshold,
                                                                 template=self.template,
                                                                 constantSparsity=self.constantSparsity,
                                                                 parallelInverseProduct= self.parallelInverseProduct,
                                                                 **self.kwargs)



@dataclasses.dataclass
class SimulationSettingsParameters(BaseNodeModifierParameters):
    enableCollisionDetection : bool = False
    useLagrangian : bool = False
    displayFlags : str = "showVisualModels"
    backgroundColor : list[float] = dataclasses.field(default_factory= [0.8,0.8,0.8,1])

    #Collision detection specific
    parallelComputing : bool = True
    alarmDistance : float = DEFAULT_VALUE
    contactDistance : float = DEFAULT_VALUE
    stick : bool = False

    #Lagrangian specific
    frictionCoef : float = DEFAULT_VALUE
    tolerance : float = DEFAULT_VALUE
    maxIterations : int = DEFAULT_VALUE

    _numberOfAffectedNodes = 1

    @staticmethod
    def _addConstraintCorrectionToMechanicalNodes(node, constraintCorrectionType : str = "LinearSolverConstraintCorrection"):
        #Might be weak if we only have mapped mass or forcefield. It might be better to rethink this
        for child in node.children():
            if child.getMass is not None or child.getForceField(0) is not None:
                child.addObject(constraintCorrectionType)
            else:
                SimulationSettingsParameters._addConstraintCorrectionToMechanicalNodes(child, constraintCorrectionType)

    def modifier(self, owner, node : list[Node]):
        if(self.useLagrangian):
            splib.simulation.headers.setupLagrangianCollision(node[0],
                                                              enableCollision=self.enableCollisionDetection,
                                                              displayFlags = self.displayFlags,
                                                              backgroundColor = self.backgroundColor,
                                                              parallelComputing = self.parallelComputing,
                                                              alarmDistance=self.alarmDistance,
                                                              contactDistance=self.contactDistance,
                                                              frictionCoef=self.frictionCoef,
                                                              tolerance=self.tolerance,
                                                              maxIterations=self.maxIterations,
                                                              stick = self.stick,
                                                              **self.kwargs)
            SimulationSettingsParameters._addConstraintCorrectionToMechanicalNodes(node[0], "LinearSolverConstraintCorrection")


        elif self.enableCollisionDetection:
            splib.simulation.headers.setupPenalityCollisionHeader(node[0],
                                                                  displayFlags = self.displayFlags,
                                                                  backgroundColor = self.backgroundColor,
                                                                  parallelComputing = self.parallelComputing,
                                                                  alarmDistance=self.alarmDistance,
                                                                  contactDistance=self.contactDistance,
                                                                  stick = self.stick,
                                                                  **self.kwargs)
        else:
            splib.simulation.headers.setupDefaultHeader(node[0],
                                                        displayFlags = self.displayFlags,
                                                        backgroundColor = self.backgroundColor,
                                                        parallelComputing = self.parallelComputing,
                                                        **self.kwargs)