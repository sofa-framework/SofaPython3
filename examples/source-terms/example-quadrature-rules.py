"""Solves a linear elastic problem with a prescribed load on the RHS. The load
is assembled from a manufactured body-force density. The assembly is done by
integrating the density using quadrature information provided by the SofaFEM
quadrature bindings.

The resulting displacement should match the manufactured solution:
ue(x,y) = 0.03 * (sin(2 pi x) sin(2 pi y), sin(2 pi x) sin(2 pi y)) on [0,1]^2,
with zero Dirichlet BCs on all boundaries.

The mesh is coarse enough to reveal a difference in accuracy between the
quadrature degrees in use.

White: reference solution ue
Red & Green: solved using a load assembled with quadrature order 1 & 2, respectively.
"""
import numpy as np
import Sofa
import Sofa.Core
from Sofa import SofaFEM

AMPLITUDE = 0.03
WAVENUMBER = 2 * np.pi


def manufactured_body_force_density(positions, young_modulus, poisson_ratio):
    """The RHS side f of the linear elasticity PDE:
       f = Ak²[(λ+3μ) sin(kx)sin(ky) − (λ+μ) cos(kx)cos(ky)], with A=AMPLITUDE, k=WAVENUMBER
       Acts as a source term, exciting the displacement field to yield ue, the manufactured solution"""
    mu = young_modulus / (2 * (1 + poisson_ratio))
    lam = young_modulus * poisson_ratio / (1 - poisson_ratio ** 2)

    x, y = positions[:, 0], positions[:, 1]
    k = WAVENUMBER
    s = AMPLITUDE * np.sin(k * x) * np.sin(k * y)
    c = AMPLITUDE * np.cos(k * x) * np.cos(k * y)
    f = k ** 2 * ((lam + 3 * mu) * s - (lam + mu) * c)
    return np.column_stack([f, f])


def assemble_nodal_force(node_coordinates, node_indices, quadrature_degree, nodal_density):
    """Assembles a per-node force vector by integrating a nodal source density:
    F_a = sum_q w_q * measure_q * N_a(q) * r(q), with r(q) = sum_a N_a(q) * density_a"""

    # First get the quadrature data and mapping information for the corresponding topological element
    weights, shape_functions, reference_gradients = SofaFEM.quadrature_data("Triangle", 2, quadrature_degree)
    _, measures = SofaFEM.element_mapping_batch("Triangle", node_coordinates, node_indices, reference_gradients)

    element_density = nodal_density[node_indices]  # density at each element's local nodes, (E, nbNodes, dof)
    nb_nodes_per_element = node_indices.shape[1]

    nodal_force = np.zeros_like(nodal_density)
    for weight, measure, shape_function in zip(weights, measures.T, shape_functions):
        # r(q), per element: interpolate the local nodal densities with the shape functions
        density_at_q = sum(shape_function[a] * element_density[:, a] for a in range(nb_nodes_per_element))

        # scatter w_q * measure_q * N_a(q) * r(q) onto each local node a
        for a in range(nb_nodes_per_element):
            contribution = weight * measure * shape_function[a]  # (E,)
            np.add.at(nodal_force, node_indices[:, a], contribution[:, None] * density_at_q)
    return nodal_force


class AssembleSourceTerms(Sofa.Core.Controller):
    """Assembles the manufactured body-force load for and assigns it to the Data of a ConstantForceField."""
    def __init__(self, node, quadrature_degree, young_modulus, poisson_ratio, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.node = node
        self.quadrature_degree = quadrature_degree
        self.young_modulus = young_modulus
        self.poisson_ratio = poisson_ratio

    def onSimulationInitDoneEvent(self, _):
        node_coordinates = self.node.dofs.rest_position.array()
        node_indices = np.array(self.node.Triangle_topo.triangles.array())

        nodal_density = manufactured_body_force_density(node_coordinates, self.young_modulus, self.poisson_ratio)
        nodal_force = assemble_nodal_force(node_coordinates, node_indices, self.quadrature_degree, nodal_density)

        self.node.addObject("ConstantForceField", name="source",
                             indices=list(range(len(nodal_force))), forces=nodal_force.tolist()).init()


def createScene(root):
    root.dt = 1
    root.gravity = [0, 0, 0]

    root.addObject("RequiredPlugin", pluginName=[
        "Sofa.Component.AnimationLoop",
        "Sofa.Component.Constraint.Projective",
        "Sofa.Component.Engine.Select",
        "Sofa.Component.LinearSolver.Direct",
        "Sofa.Component.MechanicalLoad",
        "Sofa.Component.ODESolver.Backward",
        "Sofa.Component.SolidMechanics.FEM.Elastic",
        "Sofa.Component.StateContainer",
        "Sofa.Component.Topology.Container.Dynamic",
        "Sofa.Component.Topology.Container.Grid",
        "Sofa.Component.Topology.Mapping",
        "Sofa.Component.Visual",
        "Sofa.GL.Component.Rendering2D",
    ])

    root.addObject("DefaultAnimationLoop")
    root.addObject("VisualStyle", displayFlags="showBehaviorModels showVisualModels showWireframe")

    with root.addChild("Labels") as labels:
        labels.addObject("OglLabel", name="Reference",        x="10", y="810", fontsize="16", color="1 1 1 1")
        labels.addObject("OglLabel", name="Deg1", x="10", y="832", fontsize="16", color="1 0 0 1")
        labels.addObject("OglLabel", name="Deg2", x="10", y="854", fontsize="16", color="0 1 0 1")

    quadratureDegrees = root.addChild("QuadratureDegrees")
    quadratureDegrees.addObject("RegularGridTopology", name="grid", n="5 5 1", min="0 0 0", max="1 1 0")

    # Reference Mesh: nodes are placed exactly on the manufactured solution, i.e. at grid position + ue(grid position).
    with quadratureDegrees.addChild("ManufacturedSolution") as manufacturedSolution:
        # Compute the exact displacement from the analytical expression
        grid_coordinates = np.linspace(0, 1, 5)
        grid_x, grid_y = np.meshgrid(grid_coordinates, grid_coordinates)
        displacement = AMPLITUDE * np.sin(WAVENUMBER * grid_x) * np.sin(WAVENUMBER * grid_y)
        referencePositions = np.column_stack([(grid_x + displacement).ravel(), (grid_y + displacement).ravel()])

        # Assign the computed displacement the dofs
        manufacturedSolution.addObject("MechanicalObject", name="dofs", template="Vec2", showObject="true", drawMode="1", showObjectScale="0.005", showColor="1 1 1 0.7",
                                        position=referencePositions.tolist())
        manufacturedSolution.addObject("TriangleSetTopologyContainer", name="Triangle_topo")
        manufacturedSolution.addObject("TriangleSetTopologyModifier", name="Modifier")
        manufacturedSolution.addObject("TriangleSetGeometryAlgorithms", template="Vec2", name="GeomAlgo", drawEdges="1", drawColorEdges="1 1 1 1")
        manufacturedSolution.addObject("Quad2TriangleTopologicalMapping", input="@../grid", output="@Triangle_topo", swapping="True")

    boundaryBox = ("-0.125 -0.125 -0.125  0.125 1.125 0.125   "
                   "0.875 -0.125 -0.125  1.125 1.125 0.125   "
                   "-0.125 -0.125 -0.125  1.125 0.125 0.125   "
                   "-0.125 0.875 -0.125  1.125 1.125 0.125")

    youngModulus = 100000
    poissonRatio = 0.3

    def addQuadratureDegreeNode(name, quadrature_degree, showColor, edgeColor):
        # Starts at rest, loaded by the manufactured body force, integrated with the given quadrature degree.
        with quadratureDegrees.addChild(name) as node:
            node.addObject("NewtonRaphsonSolver", name="newton", maxNbIterationsNewton="1", absoluteResidualStoppingThreshold="1e-8")
            node.addObject("StaticSolver", newtonSolver="@newton")
            node.addObject("SparseLDLSolver", name="linear_solver", template="CompressedRowSparseMatrixd")

            node.addObject("MechanicalObject", name="dofs", template="Vec2", src="@../grid", showObject="true", drawMode="1", showObjectScale="0.005", showColor=showColor)
            node.addObject("BoxROI", name="boundary", template="Vec2", box=boundaryBox)
            node.addObject("FixedProjectiveConstraint", name="dirichlet", indices="@boundary.indices")

            node.addObject("TriangleSetTopologyContainer", name="Triangle_topo")
            node.addObject("TriangleSetTopologyModifier", name="Modifier")
            node.addObject("TriangleSetGeometryAlgorithms", template="Vec2", name="GeomAlgo", drawEdges="1", drawColorEdges=edgeColor)
            node.addObject("Quad2TriangleTopologicalMapping", input="@../grid", output="@Triangle_topo", swapping="True")

            node.addObject("LinearSmallStrainFEMForceField", name="FEM", template="Vec2,Triangle",
                            youngModulus=youngModulus, poissonRatio=poissonRatio, topology="@Triangle_topo")

            node.addObject(AssembleSourceTerms(
                node=node, quadrature_degree=quadrature_degree,
                young_modulus=youngModulus, poisson_ratio=poissonRatio, name="assembleSourceTerm"))

    addQuadratureDegreeNode("QuadratureDegree1", 1, showColor="1 0 0 0.9", edgeColor="1 0 0 1")
    addQuadratureDegreeNode("QuadratureDegree2", 2, showColor="0 1 0 1", edgeColor="0 1 0 1")

    return root


def main():
    root = Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.init(root)


if __name__ == '__main__':
    main()
