
from softrobots import addCavity
from softrobots import types

def addCavity(mechanicalNode):
        ...

e = ElasticMaterialObject(file="tetrahedralMesh.gid", young=100)
addVisualModel(e, "surface.stl")
addCavity(e, name="PressureActuator"
             cavityGeometry="cavity_surface.stl", 
             behaviorType=types.SurfacePressureSensor)
             
addActuator(e, type="PneumaticCavity")


def addPneumaticActuator(e):
        return addCavity(e)... 
