# Required import for python
import Sofa
import numpy as np
import math


# Choose in your script to activate or not the GUI
USE_GUI = True


def main():
    import SofaRuntime
    import Sofa.Gui

    root = Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.initRoot(root)

    if not USE_GUI:
        import SofaQt

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

    root.addObject("RequiredPlugin", pluginName=[    'Sofa.Component.Collision.Detection.Algorithm',
    'Sofa.Component.Collision.Detection.Intersection', 'Sofa.Component.Collision.Geometry',
    'Sofa.Component.Collision.Response.Contact', 'Sofa.Component.Constraint.Projective',
    'Sofa.Component.IO.Mesh','Sofa.Component.LinearSolver.Iterative',
    'Sofa.Component.Mapping.Linear', 'Sofa.Component.Mass', 'Sofa.Component.ODESolver.Backward',
    'Sofa.Component.SolidMechanics.FEM.Elastic','Sofa.Component.StateContainer',
    'Sofa.Component.Topology.Container.Dynamic','Sofa.Component.Visual',
    'Sofa.GL.Component.Rendering3D','Sofa.Component.Constraint.Lagrangian.Correction',
    'Sofa.Component.Constraint.Lagrangian.Solver', 'Sofa.Component.MechanicalLoad',
    'Sofa.Component.LinearSolver.Direct','Sofa.Component.AnimationLoop'
    ])

    root.addObject('FreeMotionAnimationLoop')
    # Constraint solver computing the constraint/contact forces, stored in the constraint space (normal , tangential_1, tangential_2)
    constraint_solver = root.addObject('GenericConstraintSolver', maxIterations=1000, tolerance=1e-6, computeConstraintForces=True)

    root.addObject('VisualStyle', displayFlags="showCollisionModels hideVisualModels showForceFields")
    root.addObject('CollisionPipeline', name="CollisionPipeline")
    root.addObject('BruteForceBroadPhase', name="BroadPhase")
    root.addObject('BVHNarrowPhase', name="NarrowPhase")
    root.addObject('DiscreteIntersection')
    root.addObject('CollisionResponse', name="CollisionResponse", response="FrictionContactConstraint", responseParams="mu=0.1")

    root.addObject('MeshOBJLoader', name="LiverSurface", filename="mesh/liver-smooth.obj")

    liver = root.addChild('Liver')
    liver.addObject('EulerImplicitSolver', name="cg_odesolver", rayleighStiffness=0.1, rayleighMass=0.1)
    liver.addObject('SparseLDLSolver', name="linear_solver")
    liver.addObject('MeshGmshLoader', name="meshLoader", filename="mesh/liver.msh")
    liver.addObject('TetrahedronSetTopologyContainer', name="topo", src="@meshLoader")
    # Liver MechanicalObject where the constraint/contact forces will be stored in the (x,y,z) coordinate system
    liverMO = liver.addObject('MechanicalObject', name="dofs", src="@meshLoader")
    liver.addObject('TetrahedronSetGeometryAlgorithms', template="Vec3d", name="GeomAlgo")
    liver.addObject('DiagonalMass', name="Mass", massDensity=1.0)
    liver.addObject('TetrahedralCorotationalFEMForceField', template="Vec3d", name="FEM", method="large", poissonRatio=0.3, youngModulus=3000, computeGlobalMatrix=False)
    liver.addObject('FixedProjectiveConstraint', name="FixedConstraint", indices=[3,39,64])

    # Forcefield only used for visualization purposes (of the contact forces)
    CFF = liver.addObject('ConstantForceField', name="CFF", forces=[0,0,0], showArrowSize=10)
    liver.addObject('LinearSolverConstraintCorrection')

    visu = liver.addChild('Visu')
    visu.addObject('OglModel', name="VisualModel", src="@../../LiverSurface")
    visu.addObject('BarycentricMapping', name="VisualMapping", input="@../dofs", output="@VisualModel")

    surf = liver.addChild('Surf')
    surf.addObject('SphereLoader', name="sphereLoader", filename="mesh/liver.sph")
    surf.addObject('MechanicalObject', name="spheres", position="@sphereLoader.position")
    surf.addObject('SphereCollisionModel', name="CollisionModel", listRadius="@sphereLoader.listRadius")
    surf.addObject('BarycentricMapping', name="CollisionMapping", input="@../dofs", output="@spheres")


    particle = root.addChild('Particle')
    particle.addObject('EulerImplicitSolver')
    particle.addObject('CGLinearSolver', threshold=1e-09, tolerance=1e-09, iterations=200)
    # Particle MechanicalObject where the constraint/contact forces will be stored in the (x,y,z) coordinate system
    particleMO = particle.addObject('MechanicalObject', showObject=True, position=[-2, 10, 0, 0, 0, 0, 1], name=f'ParticleDoFs', template='Rigid3d')
    particle.addObject('UniformMass', totalMass=1)
    particle.addObject('ConstantForceField', name="CFF", totalForce=[0, -1, 0, 0, 0, 0] )
    particle.addObject('SphereCollisionModel', name="SCM", radius=1.0 )
    particle.addObject('UncoupledConstraintCorrection')

    # Python controller accessing and displaying the contact forces in the ConstantForceField
    root.addObject(AccessContactForces('AccessContactForces', name='AccessContactForces',
                                       constraint_solver=constraint_solver,
                                       soft_liver=liverMO,
                                       forcefield_visu=CFF,
                                       root_node=root))


class AccessContactForces(Sofa.Core.Controller):

    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.constraint_solver = kwargs.get("constraint_solver")
        self.soft_liver = kwargs.get("soft_liver")
        self.forcefield_visu = kwargs.get("forcefield_visu")
        self.root_node = kwargs.get("root_node")


    def onAnimateEndEvent(self, event):

        lambda_vector = self.constraint_solver.constraintForces.value
        if(len(lambda_vector) > 0):
            print(f"Forces in the contact space (n, t1, t2) =  {lambda_vector}  (at time = {round(self.root_node.time.value,3))})")

            self.forcefield_visu.forces.value = -self.soft_liver.getData("lambda").value
            visuScale = self.forcefield_visu.showArrowSize.value
            fact =8.0/np.max(lambda_vector)
            self.forcefield_visu.showArrowSize.value = fact
        else:
            self.forcefield_visu.forces.value = np.zeros((181,3))



# Function used only if this script is called from a python environment
if __name__ == '__main__':
    main()
