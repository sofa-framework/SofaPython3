"""
           Simulation
           -----------------------

           Example:

              .. code-block:: python

                import Sofa
                n = Sofa.Core.Node("MyNode"")
                Sofa.Simulation.init(root)
                Sofa.Simulation.print(root)

           Functions:

           .. autosummary::
               Sofa.Simulation.animate
               Sofa.Simulation.init
               Sofa.Simulation.print
               Sofa.Simulation.reset
               Sofa.Simulation.load
               Sofa.Simulation.unload
       """
from typing import *
from array import array
from typing import Iterable as iterable
from typing import Iterator as iterator
from numpy import float64
_Shape = Tuple[int, ...]
__all__  = [
"animate",
"draw",
"glewInit",
"init",
"initTextures",
"load",
"print",
"reset",
"unload",
"updateVisual"
]
def animate(arg0: Sofa.Core.Node, arg1: float) -> None:
    pass
def draw(arg0: Sofa.Core.Node) -> None:
    pass
def glewInit() -> None:
    pass
def init(arg0: Sofa.Core.Node) -> None:
    pass
def initTextures(arg0: Sofa.Core.Node) -> None:
    pass
def load(arg0: str) -> Sofa.Core.Node:
    pass
def print(arg0: Sofa.Core.Node) -> None:
    pass
def reset(arg0: Sofa.Core.Node) -> None:
    pass
def unload(arg0: Sofa.Core.Node) -> None:
    pass
def updateVisual(arg0: Sofa.Core.Node) -> None:
    pass