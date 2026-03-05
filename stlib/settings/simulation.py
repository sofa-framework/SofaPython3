import Sofa.Core
from splib.core.utils import REQUIRES_COLLISIONPIPELINE, REQUIRES_LAGRANGIANCONSTRAINTSOLVER

def addSimulationSettings(rootnode: Sofa.Core.Node):

    rootnode.add('DefaultVisualManagerLoop')
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

    simulation = rootnode.add(Sofa.Core.Node('Simulation'))

    simulation.add('EulerImplicitSolver')
    simulation.add('SparseLDLSolver')

    if rootnode.findData(REQUIRES_LAGRANGIANCONSTRAINTSOLVER) and rootnode.findData(REQUIRES_LAGRANGIANCONSTRAINTSOLVER).value:
        simulation.add('GenericConstraintCorrection')

    return simulation

