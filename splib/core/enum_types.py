from enum import Enum

class ConstitutiveLaw(Enum):
    ELASTIC         = 1
    HYPERELASTIC    = 2

class ODEType(Enum):
    EXPLICIT        = 1
    IMPLICIT        = 2

class SolverType(Enum):
    DIRECT          = 1
    ITERATIVE       = 2

class MappingType(Enum):
    BARYCENTRIC     = 1
    IDENTITY        = 2
    RIGID           = 3

class ConstraintType(Enum):
    PROJECTIVE      = 1
    WEAK            = 2
    LAGRANGIAN      = 3

class CollisionPrimitive(Enum):
    POINTS          = 1
    LINES           = 2
    TRIANGLES       = 3
    SPHERES         = 4

class ElementType(Enum):
    POINTS          = 1
    EDGES           = 2
    TRIANGLES       = 3
    QUADS           = 4
    TETRAHEDRA      = 5
    HEXAHEDRA       = 6

class StateType(Enum):
    VEC3            = 3
    VEC1            = 1
    RIGID           = 7

    def __str__(self):
        if self == StateType.VEC3:
            return "Vec3"
        if self == StateType.VEC1:
            return "Vec1"
        if self == StateType.RIGID:
            return "Rigid3"
