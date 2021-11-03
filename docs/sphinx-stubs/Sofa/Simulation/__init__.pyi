"""
        Controlling a simulation
        ------------------------
        Main controller of the scene.
        It defines how the scene is inited at the beginning, and updated at each time step.
        It derives from Base in order to use smart pointers and model the parameters as Datas,
        which makes their edition easy in the GUI.

        Example:

           .. code-block:: python

             import Sofa.Core
             import SofaRuntime
             SofaRuntime.importPlugin("SofaComponentAll")

             n = Sofa.Core.Node("MyNode")
             Sofa.Simulation.init(n)
             Sofa.Simulation.print(n)

        Functions:

        .. autosummary::
            Sofa.Simulation.animate
            Sofa.Simulation.init
            Sofa.Simulation.print
            Sofa.Simulation.reset
            Sofa.Simulation.load
            Sofa.Simulation.unload
        """
import Sofa.Simulation
import typing
import Sofa.Core

__all__ = [
    "animate",
    "init",
    "initTextures",
    "initVisual",
    "load",
    "print",
    "reset",
    "unload",
    "updateVisual"
]


def animate(arg0: Sofa.Core.Node, arg1: float) -> None:
    """
            Execute one timestep. If dt is 0, the dt parameter in the graph will be used.
            :param dt: the duration of the step.
            
    """
def init(arg0: Sofa.Core.Node) -> None:
    """
            Initialize the objects.
            
    """
def initTextures(arg0: Sofa.Core.Node) -> None:
    pass
def initVisual(arg0: Sofa.Core.Node) -> None:
    pass
def load(arg0: str) -> object:
    """
            Load a scene from a file.
            :param filename: name of the file to be loaded
            :param reload: default to false. It signals if the scene is being reloaded or loaded for
            the first time.
            :param sceneArgs: default to an empty vector. Contains the arguments of the scene.
            
    """
def print(arg0: Sofa.Core.Node) -> None:
    """
            Print all objects in the graph.
            
    """
def reset(arg0: Sofa.Core.Node) -> None:
    """
            Reset to initial state.
            
    """
def unload(arg0: Sofa.Core.Node) -> None:
    """
            Unload a scene from a Node.
            
    """
def updateVisual(arg0: Sofa.Core.Node) -> None:
    pass
