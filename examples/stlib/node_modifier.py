from stlib.geometries.plane import PlaneParameters
from stlib.geometries.file import FileParameters
from stlib.geometries.extract import ExtractParameters
from stlib.materials.deformable import DeformableBehaviorParameters
from stlib.collision import Collision, CollisionParameters
from stlib.entities import Entity, EntityParameters
from stlib.visual import Visual, VisualParameters
from splib.core.enum_types import CollisionPrimitive, ElementType, ConstitutiveLaw
from splib.simulation.headers import setupLagrangianHeader, setupDefaultHeader
from splib.simulation.ode_solvers import addImplicitODE
from splib.simulation.linear_solvers import addLinearSolver
import dataclasses
import numpy as np


from stlib.node_modifiers import NodeModifier
from stlib.node_modifiers.footers import SimulationSolversParameters, SimulationSettingsParameters
from stlib.node_modifiers.attachments import FixConstraintParameters, AttachmentConstraintParameters

def createScene(root):
    root.gravity=[0,0,9.81]
    root.dt=0.01

    ##Environement
    plane1_collisionParams = CollisionParameters()
    plane1_collisionParams.name = "UP"
    plane1_collisionParams.primitives = [CollisionPrimitive.TRIANGLES]
    plane1_collisionParams.kwargs = {"TriangleCollision" : {"moving" : False, "simulated" : False}}
    plane1_collisionParams.geometry = PlaneParameters(center=np.array([15,0,5]), 
                                                                                                      normal=np.array([0,0,-1]), 
                                                                                                      lengthNormal=np.array([0, 1, 0]),
                                                                                                      lengthNbEdge=1, 
                                                                                                      widthNbEdge=2, 
                                                                                                      lengthSize=30, 
                                                                                                      widthSize=70)
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
    SParams = EntityParameters()
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
    ## TODO: not working with static container because the init order is always wrong so that the triangle vector is always empty when initializing the container
    SParams.collision.geometry = ExtractParameters(destinationType=ElementType.TRIANGLES, sourceParameters=SParams.geometry)
    SParams.visual = VisualParameters()
    SParams.visual.geometry = FileParameters(filename="mesh/SofaScene/SVisu.obj")
    SParams.visual.color = [0.7, .7, 0.7, 0.8]

    S = ModelsNode.add(Entity, parameters = SParams)

    #TODO make the name automatically match the modifier type if none is given
    root.add(NodeModifier, on = [ModelsNode], parameters = SimulationSolversParameters(constantSparsity=False))

    root.add(NodeModifier, on = [root], parameters = SimulationSettingsParameters(displayFlags = ["showVisualModels", "showInteractionForceFields"],
                                                                                  enableCollisionDetection = True,
                                                                                  useLagrangian = True,
                                                                                  parallelComputing = False,
                                                                                  alarmDistance=0.3, contactDistance=0.02,
                                                                                  frictionCoef=0.5, tolerance=1.0e-4, maxIterations=20))

    Logo.add(NodeModifier, on = [Logo], parameters = FixConstraintParameters( boxROIs=[[-1, -2, -13, 3, 2, -7]]))
    Logo.add(NodeModifier, on = [Logo], parameters = FixConstraintParameters( boxROIs=[[-100, -2, -13, -300, 2, -7]]))
    ModelsNode.add(NodeModifier, on = [S, Logo], parameters = AttachmentConstraintParameters(name = "AttachmentConstraintParameters", indices1=[26,20,119,121], indices2=[722,732,574,573], stiffness=0.5, damping=0.0,
                                                                                             length=[((9.43-9.35)**2 + (-.44-0.48)**2 + (-6.01+6.56)**2)**(0.5) ]))

