"""Deprecated module, kept for backward compatibility. Use Sofa.Component.Constraint.Lagrangian.Solver instead."""
import Sofa.Helper

Sofa.Helper.msg_deprecated("SofaPython3", "The python module 'Sofa.SofaConstraintSolver' is deprecated and will be removed: "
                           "use 'Sofa.Component.Constraint.Lagrangian.Solver' instead.")

from Sofa.Component.Constraint.Lagrangian.Solver import *
