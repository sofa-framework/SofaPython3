"""
              Expose aspect specific to the application/runtime
              -------------------------------------------------

              .. autosummary::
                  :toctree:_autosummary/_autosummary

                  SofaRuntime.importPlugin


              Example of use:
                .. code-block:: python

                   import SofaRuntime
                   SofaRuntime.importPlugin("SofaComponentAll")

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
    """
import a sofa plugin into the current environment
"""
DataRepository = None # type: Sofa.Helper.System.FileRepository # value = <Sofa.Helper.System.FileRepository object at 0x7f0d445c04c8>
PluginRepository = None # type: Sofa.Helper.System.FileRepository # value = <Sofa.Helper.System.FileRepository object at 0x7f0d445c0538>