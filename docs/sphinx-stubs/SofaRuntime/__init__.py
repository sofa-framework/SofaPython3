"""
              SofaRuntime
              -----------------------

              Example of use:
              .. code-block:: python

              import SofaRuntime
              SofaRuntime.importPlugin("MechanicalObject"")

              .. automethod::
              :toctree: _autosummary

              SofaRuntime.importPlugin
              """
import SofaRuntime
from typing import *
from typing import Iterable as iterable
from typing import Iterator as iterator
from numpy import float64
_Shape = Tuple[int, ...]
__all__  = [
"importPlugin",
"Input",
"Timer",
"DataRepository",
"PluginRepository"
]
def importPlugin(arg0: str) -> bool:
    pass
DataRepository = None # type: Sofa.Helper.System.FileRepository # value = <Sofa.Helper.System.FileRepository object at 0x7f71ee33a1b8>
PluginRepository = None # type: Sofa.Helper.System.FileRepository # value = <Sofa.Helper.System.FileRepository object at 0x7f71ee0583b0>