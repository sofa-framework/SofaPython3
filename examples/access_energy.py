import os
import Sofa

dirname = os.path.dirname(__file__)
def createScene(rootNode, dt=0.01, m=1, g=1, L=100, mu=0):

    # rootNode
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Collision.Detection.Algorithm')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Collision.Detection.Intersection')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Collision.Geometry')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Collision.Response.Contact')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.IO.Mesh')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.LinearSolver.Iterative')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Mapping.Linear')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Mass')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.ODESolver.Backward')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.SolidMechanics.Spring')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.StateContainer')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Topology.Container.Constant')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Topology.Container.Grid')
    rootNode.addObject('RequiredPlugin', name='Sofa.GL.Component.Rendering3D')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.AnimationLoop')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Constraint.Lagrangian.Correction')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.LinearSolver.Direct')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Mapping.NonLinear')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Topology.Container.Dynamic')
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.Constraint.Lagrangian.Solver') # Needed to Use components [GenericConstraintSolver]  
    rootNode.addObject('RequiredPlugin', name='Sofa.Component.LinearSystem') # Needed to Use components [MatrixLinearSystem] 
    rootNode.addObject('RequiredPlugin', name='MultiThreading') # Needed to Use components [ParallelBVHNarrowPhase,ParallelBruteForceBroadPhase]  

    rootNode.addObject('FreeMotionAnimationLoop')
    rootNode.addObject('CollisionPipeline', verbose='0', depth='10', draw='0')
    rootNode.addObject('ParallelBruteForceBroadPhase')
    rootNode.addObject('ParallelBVHNarrowPhase')
    rootNode.addObject('MinProximityIntersection', name='Proximity', alarmDistance='10', contactDistance='0.02')
    rootNode.addObject('CollisionResponse', name='Response', response='FrictionContactConstraint', responseParams=f'mu={mu}')
    rootNode.addObject('GenericConstraintSolver', maxIterations='10', multithreading='true', tolerance='1.0e-3')

    boxTranslation = "-20 -0.9 0"
    rootNode.addObject('MeshOBJLoader', name='Loader-box', filename='mesh/cube.obj', translation=boxTranslation)

    rootNode.dt = dt

    rootNode.gravity = [g, 0, -1]

    # rootNode/Box
    Box = rootNode.addChild('Box')

    Box.addObject('EulerImplicitSolver', name='EulerImplicitScheme')
    Box.addObject('SparseLDLSolver', name='linearSolver', template='CompressedRowSparseMatrixd', linearSystem='@system')
    Box.addObject('MatrixLinearSystem', template='CompressedRowSparseMatrixd', name='system')

    Box.addObject('MechanicalObject', name='BoxDOF', template='Rigid3d', position=boxTranslation+' 0 0 0 1')
    Box.addObject('UniformMass', totalMass=m)

    # rootNode/Box/Collision
    Collision = Box.addChild('Collision')
    Collision.addObject('PointSetTopologyContainer', name='boxCollision', position=boxTranslation)
    Collision.addObject('PointSetTopologyModifier', name='Modifier')
    Collision.addObject('MechanicalObject', name='CollisionDOF', template='Vec3d')
    Collision.addObject('RigidMapping', name='MappingCollision', input='@../BoxDOF', output='@CollisionDOF', globalToLocalCoords='true')
    Collision.addObject('PointCollisionModel', name='CenterLineCollisionModel', proximity='0.02')

    # rootNode/Box/Visu
    Visu = Box.addChild('Visu')
    Visu.addObject('OglModel', name='VisualModel', color='0.7 0.7 0.7 0.8', position='@../../Loader-box.position', triangles='@../../Loader-box.triangles')
    Visu.addObject('RigidMapping', name='SurfaceMapping', input='@../BoxDOF', output='@VisualModel', globalToLocalCoords='true')
    
    Box.addObject('LinearSolverConstraintCorrection', name='ConstraintCorrection', linearSolver='@linearSolver')

    # rootNode/Floor
    Floor = rootNode.addChild('Floor')
    Floor.addObject('TriangleSetTopologyContainer', name='FloorTopology', position=f'-20 -15 -2  {L} -15 -2  {L} 15 -2  -20 15 -2', triangles='0 2 1  0 3 2')
    Floor.addObject('MechanicalObject', name='FloorDOF', template='Vec3d')
    Floor.addObject('TriangleCollisionModel', name='FloorCM', proximity='0.002', moving='0', simulated='0', color='0.3 0.3 0.3 0.1')

    return 0

def main():
    import plotext
    m, g = 1, 1
    mu = .1
    root = Sofa.Core.Node("root")
    createScene(root, dt=1e-4, m=m, g=g, mu=mu, L=1000)
    Sofa.Simulation.init(root)
    Sofa.Simulation.animate(root, root.dt.value)

    Ks, Us = [], []
    count = 0
    while root.Box.BoxDOF.position.value[0][0] < 1000/1.4142:
        K, U = root.Box.computeEnergy()
        Ks.append(K)
        Us.append(U)

        Sofa.Simulation.animate(root, root.dt.value)

        if not(count%1000):
            plotext.clear_figure()
            plotext.plot(Ks)
            plotext.plot(Us)
            plotext.plot([k+u for k,u in zip(Ks,Us)])
            plotext.theme('clear')
            plotext.show()

        count += 1
    

if __name__ == '__main__':
    main()
