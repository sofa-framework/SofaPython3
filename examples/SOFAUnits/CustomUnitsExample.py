# Choose in your script to activate or not the GUI
USE_GUI = True

from units import *
from SimulationParameters import SOFAParameters

def main():
    # Required import for python
    import Sofa
    import SofaImGui

    root = Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.initRoot(root)

    if not USE_GUI:
        for iteration in range(10):
            Sofa.Simulation.animate(root, root.dt.value)
    else:
        import Sofa.Gui
        Sofa.Gui.GUIManager.Init("myscene", "imgui")
        Sofa.Gui.GUIManager.createGUI(root, __file__)
        Sofa.Gui.GUIManager.SetDimension(1080, 1080)
        Sofa.Gui.GUIManager.MainLoop(root)
        Sofa.Gui.GUIManager.closeGUI()


def createScene(root):
    SceneUnit = SOFAParameters(s, mm, g)


    root.gravity=[0, SceneUnit(-9.81, N/kg), 0]
    root.dt=0.02

    root.addObject("RequiredPlugin", pluginName=[    'Sofa.Component.Collision.Detection.Algorithm',
    'Sofa.Component.Collision.Detection.Intersection',
    'Sofa.Component.Collision.Geometry',
    'Sofa.Component.Collision.Response.Contact',
    'Sofa.Component.Constraint.Projective',
    'Sofa.Component.IO.Mesh',
    'Sofa.Component.LinearSolver.Iterative',
    'Sofa.Component.Mapping.Linear',
    'Sofa.Component.Mass',
    'Sofa.Component.IntegrationScheme.Backward',
    'Sofa.Component.SolidMechanics.FEM.Elastic',    
    'Sofa.Component.StateContainer',
    'Sofa.Component.Topology.Container.Dynamic',
    'Sofa.Component.Visual',
    'Sofa.GL.Component.Rendering3D'
    ])

    root.addObject('DefaultAnimationLoop')

    root.addObject('VisualStyle', displayFlags="showCollisionModels")
    root.addObject('CollisionPipeline', name="CollisionPipeline")
    root.addObject('BruteForceBroadPhase', name="BroadPhase")
    root.addObject('BVHNarrowPhase', name="NarrowPhase")
    root.addObject('CollisionResponse', name="CollisionResponse", response="PenalityContactForceField")
    root.addObject('DiscreteIntersection')

    root.addObject('MeshOBJLoader', name="LiverSurface", filename="mesh/liver-smooth.obj")

    liver = root.addChild('Liver')
    liver.addObject('EulerImplicitIntegrationScheme', name="cg_odesolver", rayleighStiffness="0.1", rayleighMass="0.1")
    liver.addObject('CGLinearSolver', name="linear_solver", iterations="25", tolerance="1e-09", threshold="1e-09")
    liver.addObject('MeshGmshLoader', name="meshLoader", filename="mesh/liver.msh")
    liver.addObject('TetrahedronSetTopologyContainer', name="topo", src="@meshLoader")
    liver.addObject('MechanicalObject', name="dofs", src="@meshLoader")
    liver.addObject('TetrahedronSetGeometryAlgorithms', template="Vec3d", name="GeomAlgo")

    #You can create values that have a dimension by multiplying a float/int by a unit
    liver.addObject('TetrahedralCorotationalFEMForceField', template="Vec3d", name="FEM", method="large", poissonRatio="0.3", youngModulus=SceneUnit(3 * kPa), computeGlobalMatrix="0")
    

    #Multiplications between 'DimenssionedUnit' is supported and will affect the final unit
    liverVolume = 1.5 * dm**3 # 1L
    liverMass = 1.5 * kg
    liverDensity = liverMass/liverVolume 
    #You can print the value, the unit will show
    print(f"Liver density is {liverDensity}")
    liver.addObject('DiagonalMass', name="Mass", massDensity=SceneUnit(liverDensity))

    liver.addObject('FixedProjectiveConstraint', name="FixedConstraint", indices="3 39 64")

    visu = liver.addChild('Visu')
    visu.addObject('OglModel', name="VisualModel", src="@../../LiverSurface")
    visu.addObject('BarycentricMapping', name="VisualMapping", input="@../dofs", output="@VisualModel")

    return root


# Function used only if this script is called from a python environment
if __name__ == '__main__':
    main()
