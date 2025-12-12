# Required import for python
import Sofa
import numpy as np
import math


# Main function taking a boolean as parameter to choose whether or not to use the GUI
def main(use_gui=True):
    import SofaImGui
    import Sofa.Gui

    root = Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.initRoot(root)

    if not USE_GUI:
        import SofaQt

        for iteration in range(10):
            Sofa.Simulation.animate(root, root.dt.value)
    else:
        Sofa.Gui.GUIManager.Init("myscene", "imgui")
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
    constraint_solver = root.addObject('BlockGaussSeidelConstraintSolver', maxIterations=1000, tolerance=1e-6, computeConstraintForces=True)

    root.addObject('VisualStyle', displayFlags="showCollisionModels hideVisualModels showForceFields")
    root.addObject('CollisionPipeline', name="collision_pipeline")
    root.addObject('BruteForceBroadPhase', name="broad_phase")
    root.addObject('BVHNarrowPhase', name="narrow_phase")
    root.addObject('DiscreteIntersection')
    root.addObject('CollisionResponse', name="collision_response", response="FrictionContactConstraint", responseParams="mu=0.1")

    root.addObject('MeshOBJLoader', name="load_liver_surface", filename="mesh/liver-smooth.obj")

    liver = root.addChild('Liver')
    liver.addObject('EulerImplicitSolver', name="cg_odesolver", rayleighStiffness=0.1, rayleighMass=0.1)
    liver.addObject('SparseLDLSolver', name="linear_solver", template="CompressedRowSparseMatrixMat3x3d")
    liver.addObject('MeshGmshLoader', name="loader_liver_volume", filename="mesh/liver.msh")
    liver.addObject('TetrahedronSetTopologyContainer', name="topo", src="@loader_liver_volume")
    # Liver MechanicalObject where the constraint/contact forces will be stored in the (x,y,z) coordinate system
    liverMO = liver.addObject('MechanicalObject', name="dofs", src="@loader_liver_volume")
    liver.addObject('TetrahedronSetGeometryAlgorithms', template="Vec3d", name="geom_algo")
    liver.addObject('DiagonalMass', name="mass", massDensity=1.0)
    liver.addObject('TetrahedralCorotationalFEMForceField', template="Vec3d", name="FEM", method="large", poissonRatio=0.3, youngModulus=3000, computeGlobalMatrix=False)
    liver.addObject('FixedProjectiveConstraint', name="fixed_constraint", indices=[3,39,64])
    liver.addObject('LinearSolverConstraintCorrection')

    # Forcefield only used for visualization purposes (of the contact fborces)
    contactVisu = liver.addChild('RenderingContactForces')
    contactVisu.addObject('VisualStyle', displayFlags="showVisualModels")
    renderingForces = contactVisu.addObject('VisualVectorField', name="drawing_contact_forces", vectorScale="100", vector="@../dofs.lambda", position="@../dofs.position", color="orange", drawMode="Arrow")

    visu = liver.addChild('Visu')
    visu.addObject('OglModel', name="visual_model", src="@../../load_liver_surface")
    visu.addObject('BarycentricMapping', name="visual_mapping", input="@../dofs", output="@visual_model")

    surf = liver.addChild('Surf')
    surf.addObject('SphereLoader', name="loader_sphere_model", filename="mesh/liver.sph")
    surf.addObject('MechanicalObject', name="spheres", position="@loader_sphere_model.position")
    surf.addObject('SphereCollisionModel', name="collision_model", listRadius="@loader_sphere_model.listRadius")
    surf.addObject('BarycentricMapping', name="collision_mapping", input="@../dofs", output="@spheres")


    particle = root.addChild('Particle')
    particle.addObject('EulerImplicitSolver')
    particle.addObject('CGLinearSolver', threshold=1e-09, tolerance=1e-09, iterations=200)
    # Particle MechanicalObject where the constraint/contact forces will be stored in the (x,y,z) coordinate system
    particleMO = particle.addObject('MechanicalObject', showObject=True, position=[-2, 10, 0, 0, 0, 0, 1], name=f'particle_DoFs', template='Rigid3d')
    particle.addObject('UniformMass', totalMass=1)
    particle.addObject('ConstantForceField', name="CFF", totalForce=[0, -1, 0, 0, 0, 0] )
    particle.addObject('SphereCollisionModel', name="SCM", radius=1.0 )
    particle.addObject('UncoupledConstraintCorrection')


    # Python controller accessing and displaying the contact forces in the ConstantForceField
    root.addObject(AccessContactForces('AccessContactForces', name='AccessContactForces',
                                       constraint_solver=constraint_solver,
                                       soft_liver=liverMO,
                                       forces_visu=renderingForces,
                                       root_node=root))


class AccessContactForces(Sofa.Core.Controller):

    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.constraint_solver = kwargs.get("constraint_solver")
        self.soft_liver = kwargs.get("soft_liver")
        self.forces_visu = kwargs.get("forces_visu")
        self.root_node = kwargs.get("root_node")
        # Initialize the rendered vector with zero vec<Vec3>
        self.forces_visu.vector.value = np.zeros((181,3))

    def onAnimateEndEvent(self, event):

        lambda_vector = self.constraint_solver.constraintForces.value
        # If there is a contact
        if(len(lambda_vector) > 0):
            print(f"At time = {round(self.root_node.time.value,3)}, forces in the contact space (n, t1, t2) equals:\n  {lambda_vector} ")

            # Compute the inverse (reaction force)
            self.forces_visu.vector.value = -self.soft_liver.getData("lambda").value

            # Scale automatically the displayed force vector
            visuScale = self.forces_visu.vectorScale.value
            fact = np.max(lambda_vector)
            self.forces_visu.vectorScale.value = 10/fact

        # If no contact
        else:
            self.forces_visu.vector.value = np.zeros((181,3))



# Function used only if this script is called from a python environment
if __name__ == '__main__':
    main(True)
