

def main():
    # Required import for python
    import Sofa
    import SofaImGui

    root = Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.initRoot(root)

    for iteration in range(10):
        Sofa.Simulation.animate(root, root.dt.value)
        print(root.PlateMecha.Visual.BoxROI.trianglesInROI.value)

def createScene(root_node):
    root_node.name = "root"
    root_node.dt = 0.1
    root_node.gravity = [-0.981, 0, 0]

    plugins = root_node.addChild('plugins')

    plugins.addObject('RequiredPlugin', name="MultiThreading")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.AnimationLoop")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.Collision.Detection.Algorithm")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.Collision.Detection.Intersection")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.Collision.Geometry")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.Collision.Response.Contact")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.Constraint.Lagrangian.Correction")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.Constraint.Lagrangian.Solver")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.IO.Mesh")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.LinearSolver.Direct")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.Mapping.Linear")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.Mass")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.ODESolver.Backward")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.SolidMechanics.FEM.Elastic")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.StateContainer")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.Topology.Container.Dynamic")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.Topology.Container.Grid")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.Topology.Mapping")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.Visual")
    plugins.addObject('RequiredPlugin', name="Sofa.GL.Component.Rendering3D")
    plugins.addObject('RequiredPlugin', name="Sofa.Component.Engine.Select")
    plugins.addObject('RequiredPlugin', name="Sofa.GUI.Component")

    root_node.addObject('VisualStyle', displayFlags="showVisual")
    root_node.addObject('ConstraintAttachButtonSetting')
    root_node.addObject('FreeMotionAnimationLoop', computeBoundingBox=False)
    root_node.addObject('BlockGaussSeidelConstraintSolver', maxIterations=1000, tolerance=1.0e-6)
    root_node.addObject('CollisionPipeline', name="Pipeline")
    root_node.addObject('ParallelBruteForceBroadPhase', name="BroadPhase")
    root_node.addObject('ParallelBVHNarrowPhase', name="NarrowPhase")
    root_node.addObject('CollisionResponse', name="ContactManager", response="FrictionContactConstraint", responseParams="mu=0.4")
    root_node.addObject('CCDTightInclusionIntersection', name="Intersection", continuousCollisionType="FreeMotion",maxIterations=1000,  alarmDistance=0.0, contactDistance=0.0)
    # root_node.addObject('NewProximityIntersection', name="Intersection", alarmDistance="0.5", contactDistance="0.0")


    #Liver mecha
    Liver = root_node.addChild('Liver')

    Liver.addObject('EulerImplicitSolver')
    Liver.addObject('SparseLDLSolver', name="ldl", template="CompressedRowSparseMatrixMat3x3", parallelInverseProduct=True)
    Liver.addObject('MeshGmshLoader', name="meshLoader", filename="mesh/liver.msh", scale3d=[0.2, 0.2, 0.2], rotation=[0, 180, -45], translation=[15.2, -0.15, 0.14])
    Liver.addObject('TetrahedronSetTopologyContainer', name="Container", src='@meshLoader')
    Liver.addObject('TetrahedronSetTopologyModifier', name="Modifier")
    Liver.addObject('MechanicalObject', name="mstate", template="Vec3d", position="@Container.position", velocity=[-5,0,0]*181)
    Liver.addObject('TetrahedronFEMForceField', name="forceField", listening=True, youngModulus=1e2, poissonRatio=0.3, method="large")
    Liver.addObject('MeshMatrixMass', totalMass=3)

    surface = Liver.addChild('Surface')
    surface.addObject('TriangleSetTopologyContainer', name="Container")
    surface.addObject('TriangleSetTopologyModifier', name="Modifier")
    surface.addObject('Tetra2TriangleTopologicalMapping', input="@../Container", output="@Container", flipNormals=False)
    surface.addObject('MechanicalObject', name="dofs", rest_position="@../mstate.rest_position")
    surface.addObject('PointCollisionModel', name="Collision", contactDistance=0.0001, color=[0.0, 0.93725490196078, 0.89411764705882, 1])
    surface.addObject('IdentityMapping', name="SurfaceMapping")

    visual = Liver.addChild('Visual')
    visual.addObject('TriangleSetTopologyContainer', name="Container")
    visual.addObject('TriangleSetTopologyModifier', name="Modifier")
    visual.addObject('Tetra2TriangleTopologicalMapping', input="@../Container", output="@Container", flipNormals=False)
    visual.addObject('OglModel', name="VisualModel", src="@Container", color=[0.5, 0, 0.125, 1])
    visual.addObject('IdentityMapping', name="SurfaceMapping")

    Liver.addObject('LinearSolverConstraintCorrection', linearSolver="@ldl")



    #Plate topology
    PlateTopo = root_node.addChild('BeamDomainFromGridTopology')

    PlateTopo.addObject('RegularGridTopology', name="HexaTop", n=[3, 10, 10], min=[-0.3, -1.5, -1.5], max=[0.3, 1.5, 1.5])
    tetra_topology = PlateTopo.addChild('TetraTopology')

    tetra_topology.addObject('TetrahedronSetTopologyContainer', name="Container", position="@HexaTop.position")
    tetra_topology.addObject('TetrahedronSetTopologyModifier', name="Modifier")
    tetra_topology.addObject('Hexa2TetraTopologicalMapping', input="@HexaTop", output="@Container", swapping=True)

    #Plate mecha
    PlateMecha = root_node.addChild('PlateMecha')

    PlateMecha.addObject('EulerImplicitSolver')
    PlateMecha.addObject('SparseLDLSolver', name="ldl", template="CompressedRowSparseMatrixMat3x3", parallelInverseProduct=True)
    PlateMecha.addObject('TetrahedronSetTopologyContainer', name="Container", position="@../BeamDomainFromGridTopology/HexaTop.position", tetrahedra="@../BeamDomainFromGridTopology/TetraTopology/Container.tetrahedra")
    PlateMecha.addObject('TetrahedronSetTopologyModifier', name="Modifier")
    PlateMecha.addObject('MechanicalObject', name="mstate", template="Vec3d", position="@Container.position", velocity=[15,0,0]*300)
    PlateMecha.addObject('TetrahedronFEMForceField', name="forceField", listening=True, youngModulus=2e3, poissonRatio=0.3, method="large")
    PlateMecha.addObject('MeshMatrixMass', totalMass=2)

    visual = PlateMecha.addChild('Visual')
    visual.addObject('TriangleSetTopologyContainer', name="Container")
    visual.addObject('TriangleSetTopologyModifier', name="Modifier")
    visual.addObject('Tetra2TriangleTopologicalMapping', input="@../Container", output="@Container", flipNormals=False)
    visual.addObject('BoxROI', name="FrontCollisionTriangles", box=[0.25, 1.6, 1.6, 0.35, -1.6, -1.6])
    visual.addObject('BoxROI', name="BackCollisionTriangles", box=[-0.35, 1.6, 1.6, -0.25, -1.6, -1.6])

    visual.addObject('OglModel', name="VisualModel", src="@Container" , color=[0.6, 1, 1, 1])
    visual.addObject('IdentityMapping', name="SurfaceMapping")


    surface = PlateMecha.addChild('FrontSurface')
    surface.addObject('TriangleSetTopologyContainer',triangles="@../Visual/FrontCollisionTriangles.trianglesInROI")
    surface.addObject('TriangleSetTopologyModifier', name="Modifier")
    surface.addObject('MechanicalObject', name="dofs", rest_position="@../mstate.rest_position")
    surface.addObject('TriangleCollisionModel', name="Collision",group=1, contactDistance=0.000001, color=[0.94117647058824, 0.93725490196078, 0.89411764705882, 1])
    surface.addObject('IdentityMapping', name="SurfaceMapping")



    surface = PlateMecha.addChild('BackCollisionEdges')
    surface.addObject('TriangleSetTopologyContainer',triangles="@../Visual/BackCollisionTriangles.trianglesInROI")
    surface.addObject('TriangleSetTopologyContainer', name="Modifier")
    surface.addObject('MechanicalObject', name="dofs", rest_position="@../mstate.rest_position")
    surface.addObject('PointCollisionModel', name="Collision", group=1, contactDistance=0.000001, color=[0.94117647058824, 0.93725490196078, 0.89411764705882, 1])
    surface.addObject('IdentityMapping', name="SurfaceMapping")


    PlateMecha.addObject('LinearSolverConstraintCorrection', linearSolver="@ldl")



    #Ground
    Ground = root_node.addChild('Ground')

    Ground.addObject('TriangleSetTopologyContainer', name="FloorTopology", position="-5 -15 -15  -5 -15 15  -5 15 15  -5 15 -15", triangles="0 2 1  0 3 2")
    Ground.addObject('MechanicalObject', name="FloorDOF", template="Vec3d")
    Ground.addObject('TriangleCollisionModel', name="FloorCM", contactDistance="0.0001", moving="0", simulated="0", color="0.3 0.3 0.3 0.1")
    visu = Ground.addChild('Visu')
    visu.addObject('OglModel', name="VisualModel", src="@../FloorTopology")

if __name__ == "__main__":
    main()
