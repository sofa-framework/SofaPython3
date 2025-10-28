from stlib.geometries.plane import PlaneParameters
from stlib.collision import CollisionParameters
from stlib.collision import Collision
from splib.core.enum_types import CollisionPrimitive
import dataclasses
import numpy as np



def createScene(root):
    ##Solvers



    ##Environement

    planes_lengthNormal = np.array([])
    planes_length = 1
    planes_width = 1
    planes_nbEdgeLength = 1
    planes_nbEdgeWidth = 1

    plane1_collisionParams = CollisionParameters()
    plane1_collisionParams.name = "UP"
    plane1_collisionParams.primitives = [CollisionPrimitive.TRIANGLES]
    plane1_collisionParams.kwargs = {"TriangleCollision" : {"moving" : False, "simulated" : False}}
    plane1_collisionParams.geometry = PlaneParameters(np.array([0,0,0]), np.array([0,0,-1]),
                                                      planes_lengthNormal, planes_length, planes_width, planes_nbEdgeLength, planes_nbEdgeWidth)
    root.add(Collision, plane1_collisionParams)


    plane2_collisionParams = CollisionParameters()
    plane2_collisionParams.name = "DOWN"
    plane2_collisionParams.primitives = [CollisionPrimitive.TRIANGLES]
    plane2_collisionParams.kwargs = {"TriangleCollision" : {"moving" : False, "simulated" : False}}
    plane2_collisionParams.geometry = PlaneParameters(np.array([0,0,0]), np.array([0,0,1]),
                                                      planes_lengthNormal, planes_length, planes_width, planes_nbEdgeLength, planes_nbEdgeWidth)
    root.add(Collision, plane2_collisionParams)


    ## Real models
