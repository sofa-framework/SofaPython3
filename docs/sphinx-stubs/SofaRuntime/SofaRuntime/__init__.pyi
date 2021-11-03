"""
              Expose aspect specific to the application/runtime
              -------------------------------------------------

              .. autosummary::
                  :toctree:_autosummary/_autosummary

                  SofaRuntime.importPlugin


              Example of use:
                .. code-block:: python

                   import SofaRuntime
                   SofaRuntime.importPlugin("SofaSparseSolver")

              """
import SofaRuntime.SofaRuntime
import typing
import Sofa.Helper.System

__all__ = [
    "DataRepository",
    "PluginRepository",
    "Timer",
    "importPlugin",
    "init"
]


def importPlugin(arg0: str) -> bool:
    """
    import a sofa plugin into the current environment
    """
def init() -> None:
    pass
DataRepository: Sofa.Helper.System.FileRepository
PluginRepository: Sofa.Helper.System.FileRepository
