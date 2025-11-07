"""
Toy example of a force field leveraging autodiff with JAX.

JAX can be installed via e.g. `pip install -U jax[cuda12]`
"""
import jax
import jax.numpy as jnp
import numpy as np

import Sofa


# Some configuration for JAX: device and precision
jax.config.update("jax_default_device", jax.devices("gpu")[0])  # default "gpu"
jax.config.update("jax_enable_x64", True)  # default False (ie use float32)


@jax.jit  # JIT (just-in-time compilation) for better performance
def get_force(position, length, stiffness):
    """
    Spring between the origin and the given position.

    position: array of shape (n_particles, n_dimensions)
    length: scalar or array of shape (n_particles, 1)
    stiffness: scalar or array of shape (n_particles, 1)
    """
    distance = jnp.sqrt(jnp.sum(position**2, axis=1, keepdims=True))
    direction = position / distance
    return - stiffness * (distance - length) * direction


@jax.jit  # JIT (just-in-time compilation) for better performance
def get_dforce(position, length, stiffness, vector):
    """
    Compute the jacobian-vector product (jvp) using autodiff
    """
    def get_force_from_position(x):
        return get_force(x, length, stiffness)
    # Differentiate get_force() as a function of the position
    return jax.jvp(get_force_from_position, (position,), (vector,))[1]


@jax.jit  # JIT (just-in-time compilation) for better performance
def get_kmatrix(position, length, stiffness):
    """
    Compute the jacobian using autodiff

    Warning: The jacobian computed this way is a dense matrix.
             Check `sparsejac` if you are interested in sparse jacobian with JAX.
    """
    def get_force_from_position(x):
        return get_force(x, length, stiffness)
    # Differentiate get_force() as a function of the position
    return jax.jacrev(get_force_from_position)(position)


class JaxForceField(Sofa.Core.ForceFieldVec3d):

    def __init__(self, length, stiffness, *args, **kwargs):
        Sofa.Core.ForceFieldVec3d.__init__(self, *args, **kwargs)
        self.length = length
        self.stiffness = stiffness

    def addForce(self, mechanical_parameters, out_force, position, velocity):
        with out_force.writeableArray() as wa:
            wa[:] += get_force(position.value, self.length, self.stiffness)

    def addDForce(self, mechanical_parameters, df, dx):
        with df.writeableArray() as wa:
            wa[:] += get_dforce(self.mstate.position.value, self.length, self.stiffness, dx.value) * mechanical_parameters['kFactor']

    def addKToMatrix(self, mechanical_parameters, n_particles, n_dimensions):
        return get_kmatrix(self.mstate.position.value, self.length, self.stiffness)


def createScene(root, method="implicit-matrix-assembly", n_particles=1_000, use_sofa=False):
    root.dt = 1e-3
    root.gravity = (0, -9.8, 0)
    root.box = (-5, -5, -5, 5, 5, 5)
    root.addObject(
        "RequiredPlugin",
        pluginName=[
            'Sofa.Component.Visual',
            'Sofa.Component.ODESolver.Forward',
            'Sofa.Component.ODESolver.Backward',
            'Sofa.Component.LinearSolver.Iterative',
            'Sofa.Component.LinearSolver.Direct',
            'Sofa.Component.StateContainer',
            'Sofa.Component.Mass',
            'Sofa.Component.SolidMechanics.FEM.Elastic',
            'Sofa.Component.SolidMechanics.Spring',
        ]
    )
    root.addObject("DefaultAnimationLoop")
    root.addObject("VisualStyle", displayFlags="showBehaviorModels showForceFields")

    physics = root.addChild("Physics")

    if method.lower() == "explicit":  # Requires the implementation of 'addForce'
        physics.addObject("EulerExplicitSolver", name="eulerExplicit")
    elif method.lower() == "implicit-matrix-free":  # Requires the implementation of 'addForce' and 'addDForce'
        physics.addObject("EulerImplicitSolver", name="eulerImplicit")
        physics.addObject("CGLinearSolver", template="GraphScattered", name="solver", iterations=50, tolerance=1e-5, threshold=1e-5)
    elif method == "implicit-matrix-assembly":  # Requires the implementation of 'addForce', 'addDForce' and 'addKToMatrix'
        physics.addObject("EulerImplicitSolver", name="eulerImplicit")
        physics.addObject("SparseLDLSolver", name="solver", template="CompressedRowSparseMatrixd")

    position = np.random.uniform(-1, 1, (n_particles, 3))
    velocity = np.zeros_like(position)
    length = np.random.uniform(0.8, 1.2, size=(n_particles, 1))
    stiffness = 100.0

    particles = physics.addChild("Particles")
    particles.addObject("MechanicalObject", name="state", template="Vec3d", position=position, velocity=velocity, showObject=True)
    particles.addObject("UniformMass", name="mass", totalMass=n_particles)

    if not use_sofa:  # Use the force field implemented with JAX
        particles.addObject(JaxForceField(length=length, stiffness=stiffness))
    else:  # Use a SOFA equivalent for comparison
        root.addObject("MechanicalObject", name="origin", template="Vec3d", position="0 0 0")
        particles.addObject("SpringForceField", name="force", object1="@/origin", object2="@/Physics/Particles/state", indices1=np.zeros(n_particles, dtype=np.int32), indices2=np.arange(n_particles), length=length, stiffness=stiffness*np.ones(n_particles), damping=np.zeros(n_particles))


def main():
    import argparse
    import SofaRuntime
    import SofaImGui
    import Sofa.Gui

    parser = argparse.ArgumentParser(description="Example of a scene using a ForceField implemented with JAX")
    parser.add_argument("--method", default="implicit-matrix-assembly", help="must be 'explicit', 'implicit-matrix-free' or 'implicit-matrix-assembly'")
    parser.add_argument("--particles", type=int, default=1000, help="number of particles (default 1000)")
    parser.add_argument("--use-sofa", action="store_true", help="use a force field from SOFA instead of the one implemented with JAX")
    args = parser.parse_args()

    root=Sofa.Core.Node("root")
    createScene(root, method=args.method, n_particles=args.particles, use_sofa=args.use_sofa)
    Sofa.Simulation.initRoot(root)

    Sofa.Gui.GUIManager.Init("myscene", "imgui")
    Sofa.Gui.GUIManager.createGUI(root, __file__)
    Sofa.Gui.GUIManager.SetDimension(1600, 900)
    Sofa.Gui.GUIManager.MainLoop(root)
    Sofa.Gui.GUIManager.closeGUI()


if __name__ == "__main__":
    main()
