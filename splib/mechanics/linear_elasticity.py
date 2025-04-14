from splib.core.node_wrapper import ReusableMethod
from splib.core.utils import DEFAULT_VALUE
from splib.core.enum_types import ElementType


@ReusableMethod
def addLinearElasticity(node,elem:ElementType,youngModulus=DEFAULT_VALUE, poissonRatio=DEFAULT_VALUE, method=DEFAULT_VALUE,**kwargs):
    match elem:
        case ElementType.EDGES:
            node.addObject("BeamFEMForceField",name="constitutiveLaw", youngModulus=youngModulus, poissonRatio=poissonRatio, method=method, **kwargs)
            return
        case ElementType.TRIANGLES:
            node.addObject("TriangleFEMForceField",name="constitutiveLaw", youngModulus=youngModulus, poissonRatio=poissonRatio, method=method,**kwargs)
            return
        case ElementType.QUAD:
            node.addObject("QuadBendingFEMForceField",name="constitutiveLaw", youngModulus=youngModulus, poissonRatio=poissonRatio, method=method,**kwargs)
            return
        case ElementType.TETRA:
            node.addObject("TetrahedronFEMForceField",name="constitutiveLaw", youngModulus=youngModulus, poissonRatio=poissonRatio, method=method,**kwargs)
            return
        case ElementType.HEXA:
            node.addObject("HexahedronFEMForceField",name="constitutiveLaw", youngModulus=youngModulus, poissonRatio=poissonRatio, method=method,**kwargs)
            return
        case _:
            print('Linear elasticity is only available for topology of type EDGES, TRIANGLES, QUADS, TETRAHEDRON, HEXAHEDRON')
            return