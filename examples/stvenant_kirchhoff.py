import Sofa
import Sofa.SofaDeformable

from Sofa.Units.Definitions import s, m, cm, N, g, kg, MPa
from Sofa.Units.UnitSystem import MechanicalUnitSystem

def createScene(root_node):

    plugins = [
        "Sofa.Component.Constraint.Projective",
        "Sofa.Component.Engine.Select",
        "Sofa.Component.LinearSolver.Direct",
        "Sofa.Component.Mapping.Linear",
        "Sofa.Component.Mass",
        "Sofa.Component.IntegrationScheme.Backward",
        "Sofa.Component.SolidMechanics.FEM.HyperElastic",
        "Sofa.Component.StateContainer",
        "Sofa.Component.Topology.Container.Dynamic",
        "Sofa.Component.Topology.Container.Grid",
        "Sofa.Component.Topology.Mapping",
        "Sofa.GL.Component.Rendering3D",
        "Sofa.Component.SolidMechanics.FEM.Elastic"
    ]
    root_node.addObject('RequiredPlugin', pluginName=plugins)

    scene_unit = MechanicalUnitSystem(s, cm, g)

    root_node.gravity = scene_unit([0, -9.81, 0], N / kg)
    root_node.dt = scene_unit(0.01, s)

    root_node.addObject('EulerImplicitIntegrationScheme', name="backwardEuler", rayleighStiffness=0.1, rayleighMass=0.1)
    root_node.addObject('SparseLDLSolver', template="CompressedRowSparseMatrixMat3x3d")
    root_node.addObject('RegularGridTopology', name="grid", min=[-5, -5, 0], max=[5, 5, 40], n=[5, 5, 20])
    root_node.addObject('MechanicalObject', template="Vec3", name="state")
    root_node.addObject('NodalMassDensity', property=scene_unit(1150, kg/m**3))
    root_node.addObject('FEMMass', template="Vec3,Hexahedron", topology="@grid")

    with root_node.addChild('tetra') as tetra:

        tetra.addObject('TetrahedronSetTopologyContainer', name="Tetra_topo")
        tetra.addObject('TetrahedronSetTopologyModifier', name="Modifier")
        tetra.addObject('TetrahedronSetGeometryAlgorithms', template="Vec3", name="GeomAlgo", drawTetrahedra="false")
        tetra.addObject('Hexa2TetraTopologicalMapping', input="@grid", output="@Tetra_topo")

        young_modulus = scene_unit(0.5, MPa)
        poisson_ratio = 0.45
        lame_parameters = Sofa.SofaDeformable.toLameParameters3D(young_modulus, poisson_ratio)
        tetra.addObject('TetrahedronHyperelasticityFEMForceField', name="FEM", topology="@Tetra_topo",
                        ParameterSet=lame_parameters, materialName="StVenantKirchhoff")

    with tetra.addChild('triangles') as triangles:

        triangles.addObject('TriangleSetTopologyContainer', name="Container")
        triangles.addObject('TriangleSetTopologyModifier')
        triangles.addObject('Tetra2TriangleTopologicalMapping', input="@../Tetra_topo", output="@Container")

    with triangles.addChild('Visu') as visu:

        visu.addObject('OglModel', name="Visual", color="red")
        visu.addObject('IdentityMapping', input="@../../../state", output="@Visual")

    root_node.addObject('BoxROI', template="Vec3", name="box_roi", box=[-6, -6, -1, 6, 6, 0.1])
    root_node.addObject('FixedProjectiveConstraint', template="Vec3", indices="@box_roi.indices")


def main():
    import Sofa
    import SofaImGui

    root = Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.initRoot(root)

    import Sofa.Gui
    Sofa.Gui.GUIManager.Init("myscene", "imgui")
    Sofa.Gui.GUIManager.createGUI(root, __file__)
    Sofa.Gui.GUIManager.SetDimension(1080, 1080)
    Sofa.Gui.GUIManager.MainLoop(root)
    Sofa.Gui.GUIManager.closeGUI()


if __name__ == '__main__':
    main()
