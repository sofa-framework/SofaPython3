# Required import for python
import Sofa
import numpy as np


def main():
    import SofaRuntime
    import Sofa.Gui

    root = Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.init(root)

    Sofa.Gui.GUIManager.Init("myscene", "qglviewer")
    Sofa.Gui.GUIManager.createGUI(root, __file__)
    Sofa.Gui.GUIManager.SetDimension(1080, 600)
    Sofa.Gui.GUIManager.MainLoop(root)
    Sofa.Gui.GUIManager.closeGUI()


def createScene(root):
    root.gravity=[0, -9.81, 0]
    root.dt=0.01

    root.addObject('DefaultAnimationLoop')
    root.addObject('DefaultVisualManagerLoop')
    root.addObject('VisualStyle', displayFlags="hideCollisionModels showVisualModels hideForceFields showBehaviorModels")
    root.addObject('RequiredPlugin', pluginName="Sofa.Component.Constraint.Projective Sofa.Component.Diffusion Sofa.Component.Engine.Select Sofa.Component.LinearSolver.Direct Sofa.Component.LinearSolver.Iterative Sofa.Component.Mass Sofa.Component.ODESolver.Backward Sofa.Component.SolidMechanics.FEM.Elastic Sofa.Component.Topology.Container.Dynamic Sofa.Component.Topology.Container.Grid Sofa.Component.Topology.Mapping Sofa.Component.Visual Sofa.GL.Component.Engine Sofa.GL.Component.Rendering2D Sofa.GL.Component.Rendering3D")

    root.addObject('RegularGridTopology', name="gridGenerator", nx=16, ny=6, nz=6, xmin=0, xmax=1, ymin=0, ymax=0.2, zmin=0, zmax=0.2)
    root.addObject("OglColorMap", legendTitle="A-dimensional temperature", min=0, max=1, showLegend=True, colorScheme="Blue to Red")


    tetraTopo = root.addChild('TetraTopology')
    tetraTopo.addObject("TetrahedronSetTopologyContainer", name="tetContainer", tags="3dgeometry")
    tetraTopo.addObject("TetrahedronSetTopologyModifier", name="Modifier")
    tetraTopo.addObject("Hexa2TetraTopologicalMapping", input="@../gridGenerator", output="@tetContainer")
    tetraTopo.addObject("MechanicalObject", name="tetraO", position="@../gridGenerator.position", tags="3dgeometry")
    tetraTopo.addObject("BoxROI", name="BoundaryCondition", box=[-0.01, -0.01, -0.01, 0.01, 0.21, 0.21])


    meca = tetraTopo.addChild("Mechanics")
    meca.addObject("EulerImplicitSolver", name="Euler Impl IntegrationScheme")
    meca.addObject("SparseLDLSolver", name="LDL LinearSolver", template="CompressedRowSparseMatrixMat3x3d")
    meca.addObject("TetrahedronSetTopologyContainer", name="tetContainer", src="@../tetContainer")
    meca.addObject("TetrahedronSetGeometryAlgorithms", name="tetGeometry", template="Vec3d")
    meca.addObject("MechanicalObject", name="MO", position="@../tetraO.position")
    meca.addObject("MeshMatrixMass", template="Vec3d,Vec3d", name="Mass", massDensity=100, topology="@tetContainer", geometryState="@MO")
    meca.addObject("FixedConstraint", name="FixedBoundaryCondition", indices="@../BoundaryCondition.indices")

    #initialization of the vector multiplying the local Young's modulus (need to activate updateStiffness=True)
    initVector = np.full((1, 576), 1.0)
    meca.addObject("TetrahedronFEMForceField", name="LinearElasticity", youngModulus=3e5, poissonRatio=0.4, computeVonMisesStress=1, showVonMisesStressPerElement=True, localStiffnessFactor=initVector, updateStiffness=True)


    thermo = tetraTopo.addChild("Thermodynamics")
    thermo.addObject("EulerImplicitSolver", name="Euler Impl IntegrationScheme", firstOrder=True)
    thermo.addObject("CGLinearSolver", name="Conjugate Gradient", iterations="1000", tolerance=1.0e-10, threshold=1.0e-30)
    thermo.addObject("TetrahedronSetTopologyContainer", name="tetContainer", src="@../tetContainer")
    thermo.addObject("TetrahedronSetGeometryAlgorithms", name="tetGeometry", template="Vec3d")
    thermo.addObject("MechanicalObject", name="Temperatures", template="Vec1d", size=576, showObject=True)
    thermo.addObject("MeshMatrixMass", template="Vec1d,Vec3d", name="Conductivity", massDensity=1.0, topology="@../tetContainer", geometryState="@../Mechanics/MO")
    thermo.addObject("FixedConstraint", name="Heating", indices=495)
    thermo.addObject("TetrahedronDiffusionFEMForceField", name="DiffusionForceField", template="Vec1d", constantDiffusionCoefficient=0.05, tagMechanics="3dgeometry", mstate="@Temperatures")

    thermoVisu = thermo.addChild("Visu")
    thermoVisu.addObject("TextureInterpolation", template="Vec1d", name="EngineInterpolation", input_states="@../Temperatures.position",  input_coordinates="@../../Mechanics/MO.position",  min_value=0.,  max_value=1.,  manual_scale=True , drawPotentiels=False,  showIndicesScale=5e-05)
    thermoVisu.addObject("OglModel", template="Vec3d", name="oglPotentiel", handleDynamicTopology="0", texcoords="@EngineInterpolation.output_coordinates" ,texturename="textures/heatColor.bmp", scale3d=[1, 1, 1], material="Default Diffuse 1 1 1 1 0.5 Ambient 1 1 1 1 0.3 Specular 0 0.5 0.5 0.5 1 Emissive 0 0.5 0.5 0.5 1 Shininess 0 45 No texture linked to the material No bump texture linked to the material ")
    thermoVisu.addObject("IdentityMapping", input="@../../Mechanics/MO", output="@oglPotentiel")

    # Add the controller
    root.addObject(ThermoMecaController(name = "MultiPhysicsController", ThermalObject=thermo.Temperatures, MechanicalForceField=meca.LinearElasticity))

    return root


class ThermoMecaController(Sofa.Core.Controller):
    def __init__(self, *args, **kwargs):
        # These are needed (and the normal way to override from a python class)
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        # Recover the pointers to the thermal object and the ForceField responsible of the linear elastic constitutive law
        self.ThermalObject = kwargs.get("ThermalObject")
        self.MechanicalForceField = kwargs.get("MechanicalForceField")

    def onAnimateEndEvent(self, event):
        time = self.getContext().getTime()

        self.temperatures = self.ThermalObject.position.array()

        if time > 0.5 :

            # Make the Young's modulus related to the temperature: E = E_init / (1 + 50*Temperature)
            with self.MechanicalForceField.localStiffnessFactor.writeableArray() as wa:
                for i in range(576) :
                    wa[i] = 1.0  / ( 1.+ 50.*self.temperatures[i][0] )

            #Enforce temperature of the node 495 (extremity of the bar)
            # Heating phase until t = 4
            if time < 4.0 :
                with self.ThermalObject.position.writeableArray() as wt:
                    wt[495][0] = 1.0
            # Cooling down phase when t > 4
            else :
                with self.ThermalObject.position.writeableArray() as wt:
                    wt[495][0] = 0.0


# Function used only if this script is called from a python environment
if __name__ == '__main__':
    main()