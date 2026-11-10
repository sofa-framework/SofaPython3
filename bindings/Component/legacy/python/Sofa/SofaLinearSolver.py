"""Deprecated module, kept for backward compatibility. Use Sofa.Component.LinearSolver.Iterative instead."""
import Sofa.Helper

Sofa.Helper.msg_deprecated("SofaPython3", "The python module 'Sofa.SofaLinearSolver' is deprecated and will be removed: "
                           "use 'Sofa.Component.LinearSolver.Iterative' instead.")

from Sofa.Component.LinearSolver.Iterative import *
