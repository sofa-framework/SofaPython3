import dataclasses
from stlib.node_modifiers import BaseNodeModifierParameters, AffectedNodes
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

    @override
    @AffectedNodes(1)
    def modify(self, owner, node : list[Node]) -> list[Node]:
        splib.simulation.ode_solvers.addImplicitODE(node[0], static=self.staticODE, **self.kwargs)
        splib.simulation.linear_solvers.addLinearSolver(node[0], iterative=self.iterative,
                                                                 iterations=self.iterations,
                                                                 tolerance=self.tolerance,
                                                                 threshold=self.threshold,
                                                                 template=self.template,
                                                                 constantSparsity=self.constantSparsity,
                                                                 parallelInverseProduct= self.parallelInverseProduct,
                                                                 **self.kwargs)
        return [owner]



@dataclasses.dataclass
class SimulationSettingsParameters(BaseNodeModifierParameters):
    enableCollisionDetection : bool = False
    useLagrangian : bool = False
    displayFlags : list[str] = dataclasses.field(default_factory=lambda: ["showVisualModels"])
    backgroundColor : list[float] = dataclasses.field(default_factory=lambda:  [0.8,0.8,0.8,1])

    #Collision detection specific
    parallelComputing : bool = True
    alarmDistance : float = DEFAULT_VALUE
    contactDistance : float = DEFAULT_VALUE
    stick : bool = False

    #Lagrangian specific
    frictionCoef : float = DEFAULT_VALUE
    tolerance : float = DEFAULT_VALUE
    maxIterations : int = DEFAULT_VALUE

    @staticmethod
    def _addConstraintCorrectionToMechanicalNodes(node, constraintCorrectionType : str = "LinearSolverConstraintCorrection", linearSolverPath=None):
        #TODO This only adds LinearSolvers to nodes that have mass of forcefield. Might be weak if we have mapped mass or forcefield. It might be better to rethink this
        modified = []
        for child in node.children:
            nodeLinearSolver = child.getLinearSolver(0)
            if nodeLinearSolver is not None :
                linearSolverPath = nodeLinearSolver.getLinkPath()

            if (child.getMass() is not None) or (child.getForceField(0) is not None):
                print(f"Adding constraint corr to node {child.getLinkPath()}")
                print(f"{child.getMass}    {child.getForceField(0) }")
                if(linearSolverPath is None):
                    raise ValueError(f"No linear solver found before {child.getLinkPath()}. Please add one to fix this.")
                child.addObject(constraintCorrectionType, linearSolver=linearSolverPath)
                modified += [child]
            else:
                modified += SimulationSettingsParameters._addConstraintCorrectionToMechanicalNodes(child, constraintCorrectionType, linearSolverPath)
        return modified

    @override
    @AffectedNodes(1)
    def modify(self, owner, node : list[Node]) -> list[Node]:
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


            nodeLinearSolver = node[0].getLinearSolver(0)
            if nodeLinearSolver is not None :
                linearSolverPath = nodeLinearSolver.getLinkPath()
            else:
                linearSolverPath = None

            touchedNodes = SimulationSettingsParameters._addConstraintCorrectionToMechanicalNodes(node[0], "LinearSolverConstraintCorrection", linearSolverPath = linearSolverPath)
            return [owner] + touchedNodes

        elif self.enableCollisionDetection:
            splib.simulation.headers.setupPenalityCollisionHeader(node[0],
                                                                  displayFlags = self.displayFlags,
                                                                  backgroundColor = self.backgroundColor,
                                                                  parallelComputing = self.parallelComputing,
                                                                  alarmDistance=self.alarmDistance,
                                                                  contactDistance=self.contactDistance,
                                                                  stick = self.stick,
                                                                  **self.kwargs)
            return [owner]

        else:
            splib.simulation.headers.setupDefaultHeader(node[0],
                                                        displayFlags = self.displayFlags,
                                                        backgroundColor = self.backgroundColor,
                                                        parallelComputing = self.parallelComputing,
                                                        **self.kwargs)
            return [owner]