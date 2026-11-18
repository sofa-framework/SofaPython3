from materials import Material

requiredPlugins = ([
    "Sofa.Component.Constraint.Projective",
    "Sofa.Component.Engine.Select",
    "Sofa.Component.IntegrationScheme.Backward",
    "Sofa.Component.LinearSolver.Direct",
    "Sofa.Component.LinearSystem",
    "Sofa.Component.Mass",
    "Sofa.Component.SolidMechanics.FEM.Elastic",
    "Sofa.Component.SolidMechanics.FEM.HyperElastic",
    "Sofa.Component.StateContainer",
    "Sofa.Component.Topology.Container.Dynamic",
    "Sofa.Component.Topology.Container.Grid",
    "Sofa.Component.Visual",
])


def createScene(root):
    root.gravity = [0, 0, 0]
    root.dt = 0.01

    with root.addChild("plugins") as plugins:
        plugins.addObject("RequiredPlugin", pluginName=requiredPlugins)

    root.addObject("DefaultAnimationLoop")
    root.addObject("VisualStyle", displayFlags="showVisualModels showBehaviorModels")

    # One row per material along y, one column per model along z
    for row, material in enumerate(("Dragon Skin 30", "Ecoflex 00-30")):
        y = -0.06 * row
        key = material.replace(" ", "").replace("-", "")
        root.addObject("Visual3DText", name=f"label{key}", text=material, position=[0, y + 0.02, -0.04], scale=0.004)

        for column, model in enumerate(("LinearElastic", "NeoHookean", "MooneyRivlin", "Ogden")):
            z = 0.03 * column
            if row == 0:
                root.addObject("Visual3DText", name=f"label{model}", text=model, position=[0, 0.03, z + 0.005], scale=0.004)

            with root.addChild(f"Beam{key}{model}") as beam:
                beam.addObject("EulerImplicitIntegrationScheme", rayleighStiffness=0.1, rayleighMass=0.1)
                beam.addObject("ConstantSparsityPatternSystem", template="CompressedRowSparseMatrixd")
                beam.addObject("EigenSparseLU", template="CompressedRowSparseMatrixd")
                beam.addObject("RegularGridTopology", name="grid", nx=9, ny=3, nz=3, min=[0, y, z], max=[0.1, y + 0.01, z + 0.01])
                beam.addObject("MechanicalObject", template="Vec3")
                beam.addObject("NodalMassDensity", property=1000)
                beam.addObject("FEMMass", template="Vec3,Hexahedron")
                beam.addObject("BoxROI", name="clampedEnd", box=[-0.0001, -1, -1, 0.0001, 1, 1])
                beam.addObject("FixedProjectiveConstraint", indices="@clampedEnd.indices")
                forceFieldData = {"symmetrizeTangent": False} if model == "LinearElastic" else None
                prefab = Material(parent=beam, material=material, model=model, template="Vec3,Hexahedron", forceFieldData=forceFieldData)

                stressEvaluator = prefab.forcefield if model == "LinearElastic" else prefab.constitutiveModel
                beam.addObject("VonMisesStress", template="Vec3,Hexahedron", topology="@grid", stressEvaluator=stressEvaluator.getLinkPath(),
                               colorMap="green yellow orange red purple #221C35")

                # Downward traction, in Pa, integrated over the quads of the free end face
                beam.addObject("BoxROI", name="freeEnd", box=[0.0999, -1, -1, 0.1001, 1, 1], quads="@grid.quads")
                with beam.addChild("Traction") as freeEnd:
                    freeEnd.addObject("QuadSetTopologyContainer", name="topology", quads="@../freeEnd.quadsInROI")
                    freeEnd.addObject("NodalSourceDensity", name="density", template="Vec3", property=[[0, -750, 0]])
                    freeEnd.addObject("VectorSourceTerm", name="source", template="Vec3,Quad", sourceDensity="@density")
                    freeEnd.addObject("FEMSourceTermIntegrator", template="Vec3,Quad", constantSources="@source")
