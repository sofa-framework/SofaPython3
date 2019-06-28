"""
    Sofa binding
    -----------------------

    Example of use:
      .. code-block:: python

         import Sofa

         n = Sofa.Core.Node("MyNode"")
         n.addChild("Node2")
         n.addObject("MechanicalObject", name="dofs")

         Sofa.Simulation.init(root)
         Sofa.Simulation.print(root)

    Submodules:
      .. autosummary::
        :toctree: _autosummary

        Sofa.Core
        Sofa.Simulation
        Sofa.Types
        Sofa.Helper

    """

from typing import *
from typing import Iterable as iterable
from typing import Iterator as iterator
from numpy import float64
_Shape = Tuple[int, ...]
__all__  = [
"Core",
"Helper",
"Simulation",
"Types"
]
