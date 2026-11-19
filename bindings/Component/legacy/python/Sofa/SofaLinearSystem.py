"""Deprecated module, kept for backward compatibility. Use Sofa.Component.LinearSystem instead."""
import Sofa.Helper

Sofa.Helper.msg_deprecated("SofaPython3", "The python module 'Sofa.SofaLinearSystem' is deprecated and will be removed: "
                           "use 'Sofa.Component.LinearSystem' instead.")

from Sofa.Component.LinearSystem import *
