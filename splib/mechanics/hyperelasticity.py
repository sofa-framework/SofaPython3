from splib.core.node_wrapper import ReusableMethod
from splib.core.utils import DEFAULT_VALUE
from splib.core.enum_types import ElementType


@ReusableMethod
def addHyperelasticity(node,elem:ElementType,materialName=DEFAULT_VALUE, parameterSet=DEFAULT_VALUE, matrixRegularization=DEFAULT_VALUE,**kwargs):
    match elem:
        case ElementType.TETRAHEDRA:
            node.addObject("TetrahedronHyperelasticityFEMForceField",name="constitutiveLaw", materialName=materialName, parameterSet=parameterSet, matrixRegularization=matrixRegularization, **kwargs)
            return
        case _:
            print('Hyperelasticity model only exist for Tetrahedron elements.')
            return