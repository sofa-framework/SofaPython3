from stlib.geometries.plane import PlaneParameters
from stlib.geometries.file import FileParameters
from stlib.geometries.extract import ExtractParameters
from stlib.materials.deformable import DeformableBehaviorParameters
from stlib.collision import Collision, CollisionParameters
from stlib.entities import Entity, EntityParameters
from stlib.visual import Visual, VisualParameters
from splib.core.enum_types import CollisionPrimitive, ElementType, ConstitutiveLaw
from splib.simulation.headers import setupLagrangianCollision, setupDefaultHeader
from splib.simulation.ode_solvers import addImplicitODE
from splib.simulation.linear_solvers import addLinearSolver
import dataclasses
import numpy as np


from stlib.node_modifiers import NodeModifier
from stlib.node_modifiers.footers import SimulationSolversParameters, SimulationSettingsParameters
from stlib.node_modifiers.attachments import FixConstraintParameters, AttachmentConstraintParameters

def createScene(root):
    root.gravity=[0,0,9.81]


    ##Environement
    planes_lengthNormal = np.array([0, 1, 0])
    planes_lengthNbEdge = 1
    planes_widthNbEdge = 2
    planes_lengthSize = 30
    planes_widthSize = 70

    plane1_collisionParams = CollisionParameters()
    plane1_collisionParams.name = "UP"
    plane1_collisionParams.primitives = [CollisionPrimitive.TRIANGLES]
    plane1_collisionParams.kwargs = {"TriangleCollision" : {"moving" : False, "simulated" : False}}
    plane1_collisionParams.geometry = PlaneParameters(np.array([15,0,1]), np.array([0,0,-1]),
                                                      planes_lengthNormal, planes_lengthNbEdge, planes_widthNbEdge, planes_lengthSize, planes_widthSize)
    plane1 = root.add(Collision, parameters = plane1_collisionParams)
    # TODO being able to reuse already loaded geometry of current prefab to add any new sub prefab
    # We need to enable to directly pass a link to an already existing prefab in place of a prefab parameter object
    plane1_visu = plane1.addChild("Visu")
    plane1_visu.addObject("OglModel", name="VisualModel", src="@../Geometry/container")


    ### Logo
    ModelsNode = root.addChild("ModelsNode")

    LogoParams = EntityParameters(name = "Logo1",
                                  geometry = FileParameters(filename="mesh/SofaScene/Logo.vtk"),
                                  material = DeformableBehaviorParameters(),
                                  collision = CollisionParameters(geometry = FileParameters(filename="mesh/SofaScene/LogoColli.sph")),
                                  visual = VisualParameters(geometry = FileParameters(filename="mesh/SofaScene/LogoVisu.obj")))

    LogoParams.geometry.elementType = ElementType.TETRAHEDRA
    LogoParams.material.constitutiveLawType = ConstitutiveLaw.ELASTIC
    LogoParams.material.parameters = [200, 0.4]
    LogoParams.material.massDensity = 0.003261
    LogoParams.collision.primitives = [CollisionPrimitive.SPHERES]
    #TODO make this flawless with spheres. Here collisions elements are not in the topology and a link is to be made between the loader and the collision object
    LogoParams.collision.kwargs = {"SphereCollision" : {"radius" : "@Geometry/loader.listRadius"}}
    LogoParams.visual.color = [0.7, .35, 0, 0.8]

    Logo = ModelsNode.add(Entity, parameters = LogoParams)



    ### S
    SParams = EntityParameters("bob.yaml")
    SParams.name = "S"
    SParams.geometry = FileParameters(filename="mesh/SofaScene/S.vtk")
    SParams.geometry.elementType = ElementType.TETRAHEDRA
    SParams.material = DeformableBehaviorParameters()
    SParams.material.constitutiveLawType = ConstitutiveLaw.ELASTIC
    SParams.material.parameters = [200, 0.45]
    SParams.material.massDensity = 0.011021
    SParams.collision = CollisionParameters()
    SParams.collision.primitives = [CollisionPrimitive.TRIANGLES]
    # # #TODO: to fix link issues for extracted geometry, it might be better to give source geometry relative link + parameters
    SParams.collision.geometry = ExtractParameters(destinationType=ElementType.TRIANGLES, sourceParameters=SParams.geometry )
    SParams.visual = VisualParameters()
    SParams.visual.geometry = FileParameters(filename="mesh/SofaScene/SVisu.obj")
    SParams.visual.color = [0.7, .7, 0.7, 0.8]

    S = ModelsNode.add(Entity, parameters = SParams)

    #TODO make the name automatically match the modifier type if none is given
    #TODO why this doesn't add itself to the modelsnode "modified by" data ???
    root.add(NodeModifier, on = [ModelsNode], parameters = SimulationSolversParameters(name = "SimulationSolvers",
                                                                                     constantSparsity=False))

    root.add(NodeModifier, on = [root], parameters = SimulationSettingsParameters(name = "SimulationSettings",
                                                                                  enableCollisionDetection = True,
                                                                                  useLagrangian = True,
                                                                                  parallelComputing = True,
                                                                                  alarmDistance=0.3, contactDistance=0.02,
                                                                                  frictionCoef=0.5, tolerance=1.0e-4, maxIterations=20))

