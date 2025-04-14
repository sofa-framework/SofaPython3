from typing import Callable, Optional, overload
import Sofa

class Parameters(object): 
    addConstitutiveLaw : Callable
    mass = 3.4

def Deformable(parent, name="Deformable"): 
    self = parent.addChild(name)
    return self

Deformable.get_parameters = Parameters 