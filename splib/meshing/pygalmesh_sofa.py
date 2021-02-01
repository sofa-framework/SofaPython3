# -*- coding: utf-8 -*-
# This file requires pygalmesh to be installed.
# The gallbladder.stl model is imported and converted the respective vtk file from https://www.ircad.fr/research/3d-ircadb-02/ under Creative Commons License. Please refer to their website for more information
# Author: paul.scheikl@kit.edu

"""
.. autosummary::
    meshVolumetricFromSurface
    loadVolumetricMesh
    addVisual
"""


from pathlib import Path

import pygalmesh


def meshVolumetricFromSurface(
    filename: str,
    lloyd: bool = False,
    odt: bool = False,
    perturb: bool = False,
    exude: bool = False,
    max_edge_size_at_feature_edges: float = 0.0,
    min_facet_angle: float = 0.0,
    max_radius_surface_delaunay_ball: float = 0.0,
    max_facet_distance: float = 0.0,
    max_circumradius_edge_ratio: float = 0.0,
    max_cell_circumradius: float = 0.0,
    verbose: bool = True,
    reorient: bool = False,
    seed: int = 0,
) -> str:
    """Converts a surface mesh (e.g. triangles from an stl) to a volume mesh consisting of tetrahedra. Saves the volume mesh to a vtk file and returns the filename as a string.
    """

    mesh = pygalmesh.generate_volume_mesh_from_surface_mesh(
        filename=filename,
        lloyd=lloyd,
        odt=odt,
        perturb=perturb,
        exude=exude,
        max_edge_size_at_feature_edges=max_edge_size_at_feature_edges,
        min_facet_angle=min_facet_angle,
        max_radius_surface_delaunay_ball=max_radius_surface_delaunay_ball,
        max_facet_distance=max_facet_distance,
        max_circumradius_edge_ratio=max_circumradius_edge_ratio,
        max_cell_circumradius=max_cell_circumradius,
        verbose=verbose,
        reorient=reorient,
        seed=seed,
    )

    output_path = Path(filename).parent / Path(Path(filename).stem + "_volumetric.vtk")
    output_filename = str(output_path)

    if Path(output_filename).is_file() and verbose:
        print(f"[INFO] {output_filename} existist and will be rewritten.")

    mesh.write(output_filename)

    return output_filename


def loadVolumetricMesh(node, volumeMeshFileName: str):
    """Load the volume mesh from the vtk file into a TopologyContainer and add an FEMforcefield
    """

    body = node.addChild("Body")
    loader = body.addObject(
        "MeshVTKLoader", name="loader", filename=volumeMeshFileName, printLog=True
    )

    body.addObject(
        "EulerImplicitSolver",
        name="TimeIntegrationScheme",
        rayleighStiffness=0.1,
        rayleighMass=0.1,
    )

    body.addObject(
        "CGLinearSolver",
        name="LinearSolver",
        iterations=25,
        tolerance=1e-09,
        threshold=1e-09,
    )
    body.addObject(
        "TetrahedronSetTopologyContainer",
        src=loader.getLinkPath(),
        name="TopologyContainer",
    )
    body.addObject(
        "MechanicalObject", template="Vec3d", name="dofs", src=loader.getLinkPath()
    )
    body.addObject(
        "TetrahedronSetGeometryAlgorithms", template="Vec3d", name="GeometryAlgorithms"
    )

    body.addObject("UniformMass", totalMass=5, name="Mass")

    body.addObject(
        "TetrahedronFEMForceField",
        template="Vec3d",
        method="large",
        name="FEMforcefield",
        poissonRatio=0.3,
        youngModulus=3000,
        computeGlobalMatrix=False,
    )

    body.addObject("FixedConstraint", name="FixedConstraint", indices=[5, 10, 4])

    return body


def addVisual(node, surfaceMeshFileName: str):
    """Add a visual model from the stl file to the body and map the surface to the body with a BarycentricMapping
    """
    visu = node.addChild("Visual")
    visu.addObject("MeshSTLLoader", name="surface_loader", filename=surfaceMeshFileName)
    visu.addObject("OglModel", src="@surface_loader")
    visu.addObject("BarycentricMapping")

    return visu


def addDefaultComponents(rootNode):
    """Add common components to the scene and return the rootNode itself, as well as child nodes for configuration and parent node for scene nodes.
    """

    configNode = rootNode.addChild("Config")
    configNode.addObject("RequiredPlugin", name="SofaMiscCollision")
    configNode.addObject("RequiredPlugin", name="SofaPython3")
    configNode.addObject("RequiredPlugin", name="SofaOpenglVisual")

    rootNode.gravity = [0.0, 0.0, -9.81]
    rootNode.dt = 1

    rootNode.addObject(
        "VisualStyle", displayFlags=["showInteractionForceFields", "showBehavior"]
    )

    rootNode.addObject("DefaultPipeline", name="CollisionPipeline", verbose=True)
    rootNode.addObject("BruteForceDetection", name="N2")
    rootNode.addObject(
        "DefaultContactManager", name="collision response", response="default"
    )
    rootNode.addObject("DiscreteIntersection")

    sceneNode = rootNode.addChild("Scene")

    return rootNode, configNode, sceneNode


def createScene(rootNode):
    rootNode, configNode, sceneNode = addDefaultComponents(rootNode)

    surfaceMeshFileName = "data/meshes/gallbladder.stl"
    volumeMeshFileName = meshVolumetricFromSurface(
        surfaceMeshFileName, min_facet_angle=30.0, odt=True
    )
    body = loadVolumetricMesh(sceneNode, volumeMeshFileName)
    addVisual(body, surfaceMeshFileName)

    return rootNode
