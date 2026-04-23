import Sofa.Core
from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters
from splib.core.utils import REQUIRES_COLLISIONPIPELINE, REQUIRES_LAGRANGIANCONSTRAINTSOLVER


class SimulationSettingsParameters(BaseParameters):
    name : str = "Simulation"


class SimulationSettings(BasePrefab):

    def __init__(self, parameters: SimulationSettingsParameters):
        BasePrefab.__init__(self, parameters)

    def init(self):
        rootnode = self.parents[0]

        rootnode.add('DefaultVisualManagerLoop')
        rootnode.add('VisualStyle')
        rootnode.add('InteractiveCamera')

        if rootnode.findData(REQUIRES_COLLISIONPIPELINE) and rootnode.findData(REQUIRES_COLLISIONPIPELINE).value:
            rootnode.add('CollisionPipeline')
            rootnode.add('RuleBasedContactManager', responseParams='mu=0', response='FrictionContactConstraint')
            rootnode.add('ParallelBruteForceBroadPhase')
            rootnode.add('ParallelBVHNarrowPhase')
            rootnode.add('LocalMinDistance', alarmDistance=5, contactDistance=1)

        if rootnode.findData(REQUIRES_LAGRANGIANCONSTRAINTSOLVER) and rootnode.findData(REQUIRES_LAGRANGIANCONSTRAINTSOLVER).value:
            rootnode.add('FreeMotionAnimationLoop')
            rootnode.add('BlockGaussSeidelConstraintSolver')
        else:
            rootnode.add('DefaultAnimationLoop')

        self.add('EulerImplicitSolver')
        self.add('SparseLDLSolver')

        if rootnode.findData(REQUIRES_LAGRANGIANCONSTRAINTSOLVER) and rootnode.findData(REQUIRES_LAGRANGIANCONSTRAINTSOLVER).value:
            self.add('GenericConstraintCorrection')
