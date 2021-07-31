# Required import for python
import Sofa


# Choose in your script to activate or not the GUI
USE_GUI = True


def main():
    import SofaRuntime
    import Sofa.Gui

    root = Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.init(root)

    if not USE_GUI:
        for iteration in range(10):
            Sofa.Simulation.animate(root, root.dt.value)
    else:
        Sofa.Gui.GUIManager.Init("myscene", "qglviewer")
        Sofa.Gui.GUIManager.createGUI(root, __file__)
        Sofa.Gui.GUIManager.SetDimension(1080, 1080)
        Sofa.Gui.GUIManager.MainLoop(root)
        Sofa.Gui.GUIManager.closeGUI()


def createScene(root):
    root.gravity=[0, -9.81, 0]
    root.dt=0.02

    root.addObject('DefaultVisualManagerLoop')
    root.addObject('DefaultAnimationLoop')

    root.addObject('VisualStyle', displayFlags="showCollisionModels hideVisualModels showForceFields")
    root.addObject('RequiredPlugin', pluginName="SofaImplicitOdeSolver SofaLoader SofaOpenglVisual SofaBoundaryCondition SofaGeneralLoader SofaGeneralSimpleFem") 
    root.addObject('DefaultPipeline', name="CollisionPipeline")
    root.addObject('BruteForceDetection', name="N2")
    root.addObject('DefaultContactManager', name="CollisionResponse", response="default")
    root.addObject('DiscreteIntersection')

    root.addObject('MeshObjLoader', name="LiverSurface", filename="mesh/liver-smooth.obj")

    liver = root.addChild('Liver')
    liver.addObject('EulerImplicitSolver', name="cg_odesolver", rayleighStiffness=0.1, rayleighMass=0.1)
    liver.addObject('CGLinearSolver', name="linear_solver", iterations=25, tolerance=1e-09, threshold=1e-09)
    liver.addObject('MeshGmshLoader', name="meshLoader", filename="mesh/liver.msh")
    liver.addObject('TetrahedronSetTopologyContainer', name="topo", src="@meshLoader")
    liver.addObject('MechanicalObject', name="dofs", src="@meshLoader")
    liver.addObject('TetrahedronSetGeometryAlgorithms', template="Vec3d", name="GeomAlgo")
    liver.addObject('DiagonalMass', name="Mass", massDensity=1.0)
    liver.addObject('TetrahedralCorotationalFEMForceField', template="Vec3d", name="FEM", method="large", poissonRatio=0.3, youngModulus=3000, computeGlobalMatrix=False)
    liver.addObject('FixedConstraint', name="FixedConstraint", indices="3 39 64")

    visu = liver.addChild('Visu')
    visu.addObject('OglModel', name="VisualModel", src="@../../LiverSurface")
    visu.addObject('BarycentricMapping', name="VisualMapping", input="@../dofs", output="@VisualModel")

    surf = liver.addChild('Surf')
    surf.addObject('SphereLoader', name="sphereLoader", filename="mesh/liver.sph")
    surf.addObject('MechanicalObject', name="spheres", position="@sphereLoader.position")
    surf.addObject('SphereCollisionModel', name="CollisionModel", listRadius="@sphereLoader.listRadius")
    surf.addObject('BarycentricMapping', name="CollisionMapping", input="@../dofs", output="@spheres")

    root.addObject(KeyPressedController(name = "SphereCreator"))

    return root


class KeyPressedController(Sofa.Core.Controller):
    """ This controller monitors new sphere objects.
    Press ctrl and the L key to make spheres falling!
    """
    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.iteration = 0

    def onKeypressedEvent(self, event):
        # Press L key triggers the creation of new objects in the scene
        if event['key']=='L':
            self.createNewSphere()
            
    def createNewSphere(self):
        root = self.getContext()
        newSphere = root.addChild('FallingSphere-'+str(self.iteration))
        newSphere.addObject('EulerImplicitSolver')
        newSphere.addObject('CGLinearSolver', threshold='1e-09', tolerance='1e-09', iterations='200')
        MO = newSphere.addObject('MechanicalObject', showObject=True, position=[-2, 10+self.iteration, 0, 0, 0, 0, 1], name=f'Particle-{self.iteration}', template='Rigid3d')
        Mass = newSphere.addObject('UniformMass', totalMass=1)
        Force = newSphere.addObject('ConstantForceField', name="CFF", totalForce=[0, -1, 0, 0, 0, 0] )
        Sphere = newSphere.addObject('SphereCollisionModel', name="SCM", radius=1.0 )
        
        newSphere.init()
        self.iteration = self.iteration+1


# Function used only if this script is called from a python environment
if __name__ == '__main__':
    main()