from stlib.geometries.plane import PlaneParameters
from stlib.collision import CollisionParameters
from stlib.collision import Collision
from stlib.visual import Visual
from splib.core.enum_types import CollisionPrimitive
from splib.simulation.headers import setupLagrangianCollision
import dataclasses
import numpy as np



def createScene(root):
    ##Solvers
    setupLagrangianCollision(root, displayFlags = "showVisualModels",backgroundColor=[0.8, 0.8, 0.8, 1],
                             parallelComputing = True,alarmDistance=0.3, contactDistance=0.02,
                             frictionCoef=0.5, tolerance=1.0e-4, maxIterations=20)

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
    #TODO being able to reuse already loaded geometry of current prefab to add any new sub prefab
    plane1_visu = plane1.addChild("Visu")
    plane1_visu.addObject("OglModel", name="VisualModel", src="@../Geometry/container")


    plane2_collisionParams = CollisionParameters()
    plane2_collisionParams.name = "DOWN"
    plane2_collisionParams.primitives = [CollisionPrimitive.TRIANGLES]
    plane2_collisionParams.kwargs = {"TriangleCollision" : {"moving" : False, "simulated" : False}}
    plane2_collisionParams.geometry = PlaneParameters(np.array([15,0,-20]), np.array([0,0,1]),
                                                      planes_lengthNormal, planes_lengthNbEdge, planes_widthNbEdge, planes_lengthSize, planes_widthSize)
    plane2 = root.add(Collision, parameters = plane2_collisionParams)
    plane2_visu = plane2.addChild("Visu")
    plane2_visu.addObject("OglModel", name="VisualModel", src="@../Geometry/container")


    ## Real models
