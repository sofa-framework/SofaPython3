import Sofa
import numpy as np

"""
RIGIDIFICATION is the process of telling a structural simulation that certain
sub-sections of an otherwise fully flexible body must maintain perfect, unbending
motion throughout the simulation. This simulates structures connected by bolts,
welds, or solid beams which prevent local deformation.

--- The Challenge ---
Standard Finite Element Analysis (FEA) assumes *all* nodes are independent
degrees of freedom (DoFs), meaning every point can stretch and bend freely.
When we force a section to be perfectly rigid, its movement is no longer
governed by local forces but by global rigid body kinematics (only 6 DoFs:
3 position, 3 orientation). We must combine these two fundamentally different
types of motion into one cohesive simulation system.

--- The Solution Strategy ---
We solve this by partitioning the geometry and using mapping techniques:
1. Partitioning: Divide the mesh nodes into two sets: a Deformable set and a Rigid set.
2. Separate Models: Run separate models for each set (deformableDoFs & rigidDoFs).
3. Constraint/Mapping (The Key): We use 'RigidMapping' to take the single,
   master rigid body motion (6 DoFs) and mathematically constrain *every* node in the
   rigidified subset to adopt that perfect rigid movement.
4. Combination: 'SubsetMultiMapping' assembles a complete state vector. It takes all
   nodes from both the deformable part and the constrained (but physically included)
   rigid nodes, allowing them to solve together as one composite "beam" body.

--- How this file is organized ---
The scene is built from small, reusable functions so that we can build TWO
beams side by side for comparison:
  * add_boundary_condition(node): fixes the nodes at the base (z close to 0)
    of whatever `node` it is attached to. Reused identically for a fully
    flexible beam and for the deformable subset of a rigidified beam.
  * add_rigidification(node, grid): does everything specific to
    rigidification -- selecting the rigid region, building the rigid body,
    the deformable/rigid partition, and the SubsetMultiMapping that ties them
    back together into `node`'s own MechanicalObject ("beamDoFs").
  * create_beam_simulation(root_node, node_name, is_rigidified): builds one
    complete beam (solver + topology + DoFs + physics), either fully
    deformable or rigidified, depending on `is_rigidified`.
  * createScene(root_node): calls create_beam_simulation() twice, once with
    is_rigidified=False and once with is_rigidified=True, so you get a fully
    flexible beam and a partially rigidified beam in the same scene, letting
    you visually and numerically compare the two behaviors.

--- Where does the physics actually live? ---
In BOTH beams, mass and the CorotationalFEMForceField are attached only to
"beam/physics", i.e. to the beam's own top-level MechanicalObject ("beamDoFs").
  * For the fully flexible beam (is_rigidified=False), "beamDoFs" IS the
    only set of DoFs -- physics acts on it directly, exactly like a standard
    FEM simulation.
  * For the rigidified beam (is_rigidified=True), "beamDoFs" also holds the
    physics, but it no longer stores free/independent positions: its values
    are entirely computed by SubsetMultiMapping (inside add_rigidification),
    from two children that have NO mass/force field of their own:
      - "deformableDoFs": positions only, plus the boundary condition.
      - "rigidifiedDoFs": positions only, entirely driven by RigidMapping
        from the master rigid body "rigidDoF".
    SubsetMultiMapping is two-way: it pulls positions from its two parents up
    into "beamDoFs" (position flow, parent -> child), and pushes the forces
    computed on "beamDoFs" (by "beam/physics") back down to those same two
    parents (force flow, child -> parent). Those forces further propagate
    through RigidMapping into the 6-DoF rigid body. So "beam/physics" is the
    single place where physics is defined for the rigidified beam too, and
    the mappings are responsible for distributing the resulting forces to the
    deformable nodes and to the rigid body.

--- Why bother going through this indirection at all? (Linear system size) ---
Because "deformableDoFs" and "rigidifiedDoFs" hold no mass/force field, the
solver never needs to build or solve for them directly. For the rigidified
beam, the linear system actually assembled and solved at each time step is
much smaller than the fully flexible beam's system: it only contains the free
deformable DoFs plus the 6 DoFs of the rigid body (instead of 3 DoFs for
every single node in the rigidified region). The FEM stiffness/mass matrices
are computed on the full mesh in "beam/physics", then PROJECTED down onto
this reduced system through the chain of mappings (SubsetMultiMapping, then
RigidMapping). If you inspect the assembled system matrix of "rigidified_beam" 
and compare it to"deformable_beam"'s, you will see it is smaller, and that it 
contains non-zero coupling terms between the deformable DoFs and the rigid body's 
6 DoFs -- this coupling is exactly the trace, in the reduced linear system, of
the FEM forces that were projected through the mappings.

--- What will you actually see if you run this scene? ---
Two beams are simulated side by side:
  * "deformable_beam": a plain, fully flexible FEM beam, fixed at its base.
    No VisualPointCloud is added for it explicitly, so it only renders
    through the default visual mapping / the "VisualGrid" wireframe.
  * "rigidified_beam": the same beam geometry, but with its middle section
    made rigid. Its two DoF subsets are color-coded:
      - NAVY BLUE spheres: the free, fully flexible "deformableDoFs" nodes.
      - PINK spheres: the "rigidifiedDoFs" nodes, rigidly attached to the
        master rigid body ("rigidDoF", also drawn as a frame via
        showObject=True).
    Under gravity, both beams are pinned at the base and will sag/bend
    downward over time. On "rigidified_beam", the navy sections (top and
    bottom, outside the rigid box) will visibly deform and curve, while the
    pink section (the middle block) will move and rotate as a single
    perfectly rigid chunk -- you should NOT see any bending, stretching, or
    relative motion between the pink spheres themselves, only the whole pink
    block translating/rotating together, connected to the bending navy
    sections at its two ends. Comparing this to "deformable_beam" (which bends
    smoothly all along its length) is exactly the point of building both.

--- Order of mapping operations matters ---
For the rigidified beam, forces are accumulated by traversing the scene graph
from the leaves/bottom (beam/physics, where forces are actually computed)
back up toward the root, i.e. bottom-to-top in the graph. Concretely:
  1. Forces are first computed on "beamDoFs" (mass + FEM force field, in
     "beam/physics").
  2. SubsetMultiMapping then propagates ("applies") those forces back down to
     its two parents: "deformableDoFs" and "rigidifiedDoFs".
  3. Only after that step does RigidMapping propagate the force received by
     "rigidifiedDoFs" further down to "rigidDoF", converting it into an
     equivalent force + torque on the rigid body.
If this order were reversed, or a mapping's contribution were applied before
all of the forces feeding into it were known, the accumulated force/torque on
"rigidDoF" (and consequently the rigid body's motion) would be wrong. SOFA's
mapping graph handles this ordering automatically as long as the parent/child
relationships declared below (who is "input" and who is "output" of each
mapping) correctly reflect this bottom-to-top dependency.
"""


def add_boundary_condition(node):
    """
    Pin the base of `node`'s own DoFs to the ground.

    Reused as-is for two very different situations:
      * the fully flexible beam, where `node` is the whole beam ("beamDoFs"),
      * the rigidified beam, where `node` is only the deformable subset
        ("deformableDoFs") -- the rigid subset does not need this, since its
        motion is entirely controlled by the master rigid body instead.
    """
    with node.addChild("boundary_condition") as boundary_condition:
        # Select the nodes at the very base of the structure (z close to 0)
        # and fix them so the whole object is anchored to the wall.
        boundary_condition.addObject('BoxROI', name="fixed_roi", template="Vec3",
                                     box=[[-2, -2, -0.1], [2, 2, 0.1]], drawROI=True)
        boundary_condition.addObject('FixedProjectiveConstraint', name="fixed", indices="@fixed_roi.indices",
                                     showObject=True)


def add_rigidification(node, grid):
    """
    Build the rigidified region attached under `node`, using `grid` (the
    beam's RegularGridTopology) to know the full set of node positions.

    This function does NOT attach any mass or force field: it only builds
    the partition (deformable vs. rigid), the master rigid body, and the
    SubsetMultiMapping that reconstructs `node`'s own "beamDoFs" from the two
    subsets. Physics is added separately, once, on `node` itself (see
    create_beam_simulation) -- see the module docstring above ("Where does
    the physics actually live?") for the full explanation.
    """
    with node.addChild("rigidification") as rigidification:
        # Define the ROI (Region of Interest): a box selecting which grid nodes
        # fall inside the region that will be made rigid.
        box_roi = rigidification.addObject('BoxROI', name="rigid", template="Vec3",
                                           box=[[-2, -2, 5], [2, 2, 15]],
                                           position=grid.position)

        # --- Build the lookup tables needed by SubsetMultiMapping below ---
        # BoxROI already split the node indices for us:
        indices_in = box_roi.indices.value  # global indices of nodes INSIDE the box  -> will become rigid
        indices_out = box_roi.indicesOut.value  # global indices of nodes OUTSIDE the box -> will stay deformable
        N = len(grid.position.value)  # total number of nodes in the whole grid

        # For every global node index, determine which "parent" DoF container
        # (subset) it will belong to once we assemble the beam's DoFs.
        all_indices = np.arange(0, N)
        mask_in = np.isin(all_indices, indices_in)

        # These IDs must match the order the two inputs are listed in
        # SubsetMultiMapping's "input" argument further down:
        #   input="@.../deformableDoFs  @.../rigidifiedDoFs"
        #          ^ parent_id_out=0     ^ parent_id_in=1
        parent_id_in = 1  # index of "rigidifiedDoFs" in the SubsetMultiMapping input list
        parent_id_out = 0  # index of "deformableDoFs" in the SubsetMultiMapping input list
        parent_ids = np.where(mask_in, parent_id_in, parent_id_out).astype(np.int32)

        # Each subset (deformable / rigid) has its own *local* numbering,
        # starting at 0, independent of the global grid numbering. This array
        # translates each global node index into its position within its subset.
        local_ids = np.full(N, -1, dtype=int)
        local_ids[indices_in] = np.arange(indices_in.size, dtype=np.int32)
        local_ids[indices_out] = np.arange(indices_out.size, dtype=np.int32)

        # SubsetMultiMapping expects a single flat array of (parent_id, local_id)
        # pairs, one pair per node of the final "beamDoFs", in the order the
        # nodes should appear. We interleave the two arrays here:
        # [parent_0, local_0, parent_1, local_1, parent_2, local_2, ...]
        index_pairs = np.empty(N * 2, dtype=parent_ids.dtype)
        index_pairs[0::2] = parent_ids  # even slots: which subset the node comes from
        index_pairs[1::2] = local_ids  # odd slots: the node's index within that subset

        # ====================================================================
        # RIGID DOFS SECTION
        # This is where the rigidified region actually gets its rigid-body
        # behavior. "rigidDoF" is a true 6-DoF rigid body (3 for position,
        # 3 for orientation) that the solver integrates directly. Every node
        # that was selected as "inside the box" (rigidifiedDoFs, positions
        # only, no physics of its own) is then forced to move exactly as if
        # it were welded to that single rigid body, via RigidMapping.
        # ====================================================================
        with rigidification.addChild("rigidDoFs") as rigidDoFs:
            # The single master rigid body: 6 DoFs (position + orientation).
            rigidDoFs.addObject('MechanicalObject', template="Rigid3", name="rigidDoF",
                                position=[[0, 0, 10, 0, 0, 0, 1]], showObject=True, showObjectScale="1")
            with rigidDoFs.addChild("rigidifiedDoFs") as rigidifiedDoFs:
                # Positions only -- these nodes have no independent motion;
                # RigidMapping below completely determines where they go.
                rigidifiedDoFs.addObject('MechanicalObject', template="Vec3", name="rigidifiedDoFs",
                                         position=box_roi.pointsInROI)
                # PINK spheres: this is the rigid subset in the renderer.
                rigidifiedDoFs.addObject('VisualPointCloud', position="@rigidifiedDoFs.position", sphereRadius=0.2,
                                         drawMode="Sphere", color="pink")

                # RigidMapping is a one-parent mapping: it forces every node in
                # "rigidifiedDoFs" (the child/output) to follow the rigid
                # motion of "rigidDoF" (the parent/input), just as if it were
                # solidly bolted to it. It also transmits forces the other
                # way: any force applied to a rigidified node is converted
                # into an equivalent force + torque on the master rigid body.
                rigidifiedDoFs.addObject('RigidMapping', globalToLocalCoords=True, input="@../rigidDoF",
                                         output="@rigidifiedDoFs", applyRestPosition=True)

        # ====================================================================
        # DEFORMABLE DOFS SECTION
        # This node does NOT contain any physics (no mass,
        # no force field). It only stores the positions of the "outside the
        # box" nodes and applies the boundary condition that pins the base in
        # place. Think of it as a plain container of positions that "beamDoFs"
        # (built from the mapping below, physics attached in
        # create_beam_simulation) will read from and push forces back into --
        # the actual elastic behavior is computed there, not here.
        # ====================================================================
        with rigidification.addChild("deformableDoFs") as deformableDoFs:
            # Positions only -- no Mass, no ForceField attached to this node.
            deformableDoFs.addObject('MechanicalObject', template="Vec3", name="deformableDoFs",
                                     position=box_roi.pointsOutROI)
            # NAVY BLUE spheres: this is the deformable subset in the renderer.
            deformableDoFs.addObject('VisualPointCloud', position="@deformableDoFs.position", sphereRadius=0.2,
                                     drawMode="Sphere", color="navy")

            add_boundary_condition(deformableDoFs)

        # ====================================================================
        # THE MAPPING (reassembling the beam's own DoFs)
        # This is the CRITICAL step. SubsetMultiMapping has TWO parents
        # (hence "Multi"):
        #   Input 0 (@../deformableDoFs/deformableDoFs):          the flexible nodes
        #   Input 1 (@../rigidDoFs/rigidifiedDoFs/rigidifiedDoFs): the rigidified nodes
        # For every node of "beamDoFs" (declared one level up, in
        # create_beam_simulation, and referenced here as "@../../beamDoFs"),
        # "indexPairs" tells the mapping which parent to read its position
        # from and which local index to use within that parent (this is
        # exactly the (parent_id, local_id) table built above).
        # Because SubsetMultiMapping works both ways, it also splits the
        # forces computed on "beamDoFs" (by "beam/physics") back out to the
        # two parents, which is how the FEM forces end up correctly driving
        # both the free deformable nodes and the rigid body. This backward
        # propagation happens bottom-to-top in the scene graph -- see "Order
        # of mapping operations matters" in the module docstring above.
        # ====================================================================
        with rigidification.addChild("mapping") as mapping:
            mapping.addObject('SubsetMultiMapping', template="Vec3,Vec3",
                              input="@../deformableDoFs/deformableDoFs @../rigidDoFs/rigidifiedDoFs/rigidifiedDoFs",
                              output="@../../beamDoFs", applyRestPosition=True,
                              indexPairs=index_pairs)


def create_beam_simulation(root_node, node_name, is_rigidified):
    """
    Build one complete beam simulation under root_node/node_name.

    Both branches share the same solver setup, the same grid topology, and
    the same "physics" (mass + FEM force field) attached to "beamDoFs".
    The only difference is HOW "beamDoFs" gets its motion:
      * is_rigidified=False: "beamDoFs" is a normal, fully free
        MechanicalObject; add_boundary_condition() pins its base directly.
        This is the plain FEM baseline, useful for comparison.
      * is_rigidified=True: "beamDoFs" position is instead entirely computed
        by add_rigidification()'s SubsetMultiMapping, from a deformable
        subset (with its own boundary condition) and a rigid subset driven
        by a master rigid body.
    """
    with root_node.addChild(node_name) as simulation:
        simulation.addObject('EulerImplicitSolver', name="odesolver", rayleighStiffness=0.1, rayleighMass=0.1)
        simulation.addObject('SparseLDLSolver', template="CompressedRowSparseMatrix")

        with simulation.addChild("beam") as beam:
            # Define the full computational domain: a regular 3D grid of nodes.
            grid = beam.addObject('RegularGridTopology', name="grid", n=[5, 5, 20],
                                  xmin=-1.5, xmax=1.5, ymin=-1.5, ymax=1.5, zmin=0, zmax=20)
            grid.init()

            # "beamDoFs" is the beam's own set of DoFs -- the ones physics
            # (further below) actually acts on, in both branches. When
            # is_rigidified=True, its position here is only a placeholder:
            # it gets overwritten every step by the SubsetMultiMapping built
            # inside add_rigidification().
            beam.addObject('MechanicalObject', template="Vec3", name="beamDoFs")

            if is_rigidified:
                # Wire up the rigid/deformable partition and the mapping
                # that drives "beamDoFs" (see add_rigidification docstring).
                add_rigidification(beam, grid)
            else:
                # No rigidification: pin the base of "beamDoFs" directly,
                # exactly like a standard, fully flexible FEM beam.
                add_boundary_condition(beam)

            # The actual physics: mass and elastic (corotational FEM) forces,
            # computed over the full hexahedral grid topology, applied to
            # "beamDoFs". This is the ONLY place where mass and a force field
            # are defined, for both the flexible and the rigidified beam.
            with beam.addChild("physics") as physics:
                physics.addObject('NodalMassDensity', property=1)
                physics.addObject('FEMMass', template="Vec3,Hexahedron", topology=grid.getLinkPath())
                physics.addObject('CorotationalFEMForceField', template="Vec3,Hexahedron", youngModulus=10000,
                                  poissonRatio=0.45, topology=grid.getLinkPath())


def createScene(root_node):
    root_node.name = "root"
    root_node.dt = 0.02
    root_node.gravity = [0, -9.81, 0]
    root_node.bbox = [[-5, -5, -5], [5, 5, 25]]

    # Load the SOFA plugins that provide the components used below
    # (solvers, mappings, mass, force fields, visualization, etc.)
    with root_node.addChild("plugins") as plugins:
        plugins.addObject('RequiredPlugin', pluginName="""
            Sofa.Component.Constraint.Projective
            Sofa.Component.Engine.Select
            Sofa.Component.LinearSolver.Direct
            Sofa.Component.Mapping.Linear
            Sofa.Component.Mapping.NonLinear
            Sofa.Component.Mass
            Sofa.Component.ODESolver.Backward
            Sofa.Component.SolidMechanics.FEM.Elastic
            Sofa.Component.StateContainer
            Sofa.Component.Topology.Container.Grid
            Sofa.Component.Visual""")

    root_node.addObject('DefaultAnimationLoop', parallelODESolving=True)
    root_node.addObject('VisualGrid')

    # Build the two beams side by side for comparison: a plain, fully
    # flexible beam, and a beam with a rigidified middle section. See the
    # module docstring above ("What will you actually see if you run this
    # scene?") for what to expect from each.
    create_beam_simulation(root_node, "deformable_beam", False)
    create_beam_simulation(root_node, "rigidified_beam", True)
