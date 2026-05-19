from splib.core.node_wrapper import ReusableMethod
from splib.core.utils import defaultValueType, DEFAULT_VALUE, isDefault
from splib.core.enum_types import ElementType


@ReusableMethod
def addMass(node, elementType:ElementType, totalMass=DEFAULT_VALUE, massDensity=DEFAULT_VALUE, lumping=DEFAULT_VALUE, topology=DEFAULT_VALUE, **kwargs):
    if (not isDefault(totalMass)) and (not isDefault(massDensity)) :
        print("[warning] You defined the totalMass and the massDensity in the same time, only taking massDensity into account")
        del kwargs["massDensity"]

    if(elementType is not None and elementType !=ElementType.POINTS and elementType !=ElementType.EDGES):
        node.addObject("MeshMatrixMass", 
                       name="mass", 
                       totalMass=totalMass, 
                       massDensity=massDensity, 
                       lumping=lumping, 
                       topology=topology,  **kwargs)
    else:
        if (not isDefault(massDensity)) :
            print("[warning] mass density can only be used on a surface or volumetric topology. Please use totalMass instead")
        if (not isDefault(lumping)) :
            print("[warning] lumping can only be set for surface or volumetric topology")

        node.addObject("UniformMass", name="mass", totalMass=totalMass, topology=topology,**kwargs)

