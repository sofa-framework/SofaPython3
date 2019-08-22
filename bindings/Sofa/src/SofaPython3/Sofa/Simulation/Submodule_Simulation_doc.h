#pragma once
namespace sofapython3::doc::simulation {

static auto Class =
        R"(
        Sofa.Simulation
        ---------------
        Main controller of the scene.
        It defines how the scene is inited at the beginning, and updated at each time step.
        It derives from Base in order to use smart pointers and model the parameters as Datas,
        which makes their edition easy in the GUI.

        Example:

           .. code-block:: python

             import Sofa.Core
             import SofaRuntime
             SofaRuntime.importPlugin("SofaAllCommonComponents")

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
        )";
static auto print =
        R"(
        Print all objects in the graph.
        )";

static auto init =
        R"(
        Initialize the objects.
        )";

static auto animate =
        R"(
        Execute one timestep. If dt is 0, the dt parameter in the graph will be used.
        :param dt: the duration of the step.
        )";

static auto reset =
        R"(
        Reset to initial state.
        )";
static auto load =
        R"(
        Load a scene from a file.
        :param filename: name of the file to be loaded
        :param reload: default to false. It signals if the scene is being reloaded or loaded for
        the first time.
        :param sceneArgs: default to an empty vector. Contains the arguments of the scene.
        )";

static auto unload =
        R"(
        Unload a scene from a Node.
        )";
}
