"""Deprecated module, kept for backward compatibility. Use Sofa.Component.SolidMechanics.Spring and Sofa.Component.SolidMechanics.FEM.Elastic instead."""
import Sofa.Helper

Sofa.Helper.msg_deprecated("SofaPython3", "The python module 'Sofa.SofaDeformable' is deprecated and will be removed: "
                           "use 'Sofa.Component.SolidMechanics.Spring' and 'Sofa.Component.SolidMechanics.FEM.Elastic' instead.")

from Sofa.Component.SolidMechanics.Spring import *
from Sofa.Component.SolidMechanics.FEM.Elastic import *
