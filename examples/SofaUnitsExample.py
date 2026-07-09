# Choose in your script to activate or not the GUI
USE_GUI = True

from Sofa.Units.Definitions import s, m, mm, dm, N, g, kg, kPa  
from Sofa.Units.UnitSystem import MechanicalUnitSystem
import numpy as np

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
    # We know the scene units are second for time, mm for length (because the file we import is in mm) and g for mass
    scene_unit = MechanicalUnitSystem(s, mm, g)

    # You can now convert any value of any unit to the one expected by SOFA without knowing it. 
    # Here we know that the gravity constant is 9.81 in SI unit (a.k.a. N/kg), we let SofaUnit convert it to the custom unit system that we defined
    root.gravity = [0, scene_unit(-9.81, N/kg), 0]
    root.dt = scene_unit(0.02, s)

    root.addObject("RequiredPlugin", pluginName=[    'Sofa.Component.Collision.Detection.Algorithm',
    'Sofa.Component.Collision.Detection.Intersection',
    'Sofa.Component.Collision.Geometry',
    'Sofa.Component.Collision.Response.Contact',
    'Sofa.Component.Constraint.Projective',
    'Sofa.Component.IO.Mesh',
    'Sofa.Component.LinearSolver.Iterative',
    'Sofa.Component.Mapping.Linear',
    'Sofa.Component.Mass',
    'Sofa.Component.ODESolver.Backward',
    'Sofa.Component.SolidMechanics.FEM.Elastic',    
    'Sofa.Component.StateContainer',
    'Sofa.Component.Topology.Container.Dynamic',
    'Sofa.Component.Visual',
    'Sofa.GL.Component.Rendering3D',
    'Sofa.Component.SolidMechanics.Spring'
    ])

    root.addObject('DefaultAnimationLoop')

    root.addObject('VisualStyle', displayFlags="showCollisionModels")

    root.addObject('MeshOBJLoader', name="LiverSurface", filename="mesh/liver-smooth.obj")

    liver = root.addChild('Liver')
    liver.addObject('EulerImplicitSolver', name="cg_odesolver")
    liver.addObject('CGLinearSolver', name="linear_solver", iterations= 25, tolerance= scene_unit(1e-9, m**2) , threshold= scene_unit(1e-9, m**2) )
    liver.addObject('MeshGmshLoader', name="meshLoader", filename="mesh/liver.msh")
    liver.addObject('TetrahedronSetTopologyContainer', name="topo", src="@meshLoader")
    liver.addObject('MechanicalObject', name="dofs", src="@meshLoader")
    liver.addObject('TetrahedronSetGeometryAlgorithms', template="Vec3d", name="GeomAlgo")

    # You can create values that have a dimension by multiplying a float/int by a unit
    liver.addObject('TetrahedralCorotationalFEMForceField', template="Vec3d", name="FEM", method="large", poissonRatio="0.3", youngModulus=scene_unit(3 * kPa), computeGlobalMatrix="0")
    

    # Multiplications between 'DimenssionedUnit' is supported and will affect the final unit
    liverVolume = 1.5 * dm**3 # 1L
    liverMass = 1.5 * kg
    liverDensity = liverMass/liverVolume 
    # You can print the value, the unit will show
    print(f"Liver density is {liverDensity}")
    liver.addObject('DiagonalMass', name="Mass", massDensity=scene_unit(liverDensity))

    # The library is also compatible with numpy array 
    # This would also work np.array([10, 1, 5 ]) * N/m
    # or classical list (but with lists, the list multiplication will fail, you need to specify the unit for each member)
    stiffness = np.array([10 * N/m, 1 * N/m, 5 * N/m])
    liver.addObject('RestShapeSpringsForceField', name="WeakConstraint", points=[3, 39, 64], stiffness=scene_unit(stiffness))

    visu = liver.addChild('Visu')
    visu.addObject('OglModel', name="VisualModel", src="@../../LiverSurface")
    visu.addObject('BarycentricMapping', name="VisualMapping", input="@../dofs", output="@VisualModel")

    return root


# Function used only if this script is called from a python environment
if __name__ == '__main__':
    main()
