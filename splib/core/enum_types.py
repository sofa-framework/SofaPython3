from enum import Enum

class ConstitutiveLaw(Enum):
    LINEAR_COROT    = 1
    HYPERELASTIC    = 2

class ODEType(Enum):
    EXPLICIT    = 1
    IMPLICIT    = 2

class SolverType(Enum):
    DIRECT    = 1
    ITERATIVE = 2

class MappingType(Enum):
    BARYCENTRIC = 1
    IDENTITY    = 2
    RIGID       = 3


class ConstraintType(Enum):
    PROJECTIVE = 1
    WEAK       = 2
    LAGRANGIAN = 3



class ElementType(Enum):
    POINTS      = 1
    EDGES       = 2
    TRIANGLES   = 3
    QUAD        = 4
    TETRA       = 5
    HEXA        = 6
