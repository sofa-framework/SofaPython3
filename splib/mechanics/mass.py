from splib.core.node_wrapper import ReusableMethod
from splib.core.utils import defaultValueType, DEFAULT_VALUE, isDefault
from splib.core.enum_types import ElementType


# TODO : use the massDensity only and deduce totalMass if necessary from it + volume
@ReusableMethod
def addMass(node,template,totalMass=DEFAULT_VALUE,massDensity=DEFAULT_VALUE,lumping=DEFAULT_VALUE,**kwargs):
    if (not isDefault(totalMass)) and (not isDefault(massDensity)) :
        print("[warning] You defined the totalMass and the massDensity in the same time, only taking massDensity into account")
        kwargs.pop('massDensity')

    if(template=="Rigid3"):
        node.addObject("UniformMass",name="mass", totalMass=totalMass, massDensity=massDensity, lumping=lumping, **kwargs)
    else:
        node.addObject("MeshMatrixMass",name="mass", totalMass=totalMass, massDensity=massDensity, lumping=lumping, **kwargs)




