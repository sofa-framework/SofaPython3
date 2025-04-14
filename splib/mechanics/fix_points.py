from splib.core.node_wrapper import ReusableMethod
from splib.core.utils import isDefault, DEFAULT_VALUE
from splib.core.enum_types import ConstraintType
from enum import Enum


##box
@ReusableMethod
def addFixation(node,type:ConstraintType,boxROIs=DEFAULT_VALUE, sphereROIs=DEFAULT_VALUE, indices=DEFAULT_VALUE, fixAll=DEFAULT_VALUE,**kwargs):
    if (isDefault(indices)):
        if(not isDefault(boxROIs)):
            node.addObject("BoxROI",name='fixedBoxROI',box=boxROIs,**kwargs)
            indices="@fixedBoxROI.indices"
        if(not isDefault(sphereROIs)):
            node.addObject("SphereROI",name='fixedSphereROI',centers=sphereROIs[0],radii=sphereROIs[1],**kwargs)
            indices="@fixedSphereROI.indices"

    match type:
        case ConstraintType.WEAK:
            node.addObject("FixedWeakConstraint",name="fixedConstraints", indices=indices, fixAll=fixAll, **kwargs)
            return
        case ConstraintType.PROJECTIVE:
            node.addObject("FixedProjectiveConstraint",name="fixedConstraints", indices=indices, fixAll=fixAll, **kwargs)
            return
        case ConstraintType.LAGRANGIAN:
            node.addObject("LagrangianFixedConstraint",name="fixedConstraints", indices=indices, fixAll=fixAll, **kwargs)
            return
        case _:
            print('Contraint type is either ConstraintType.PROJECTIVE, ConstraintType.WEAK or ConstraintType.LAGRANGIAN')
            return
