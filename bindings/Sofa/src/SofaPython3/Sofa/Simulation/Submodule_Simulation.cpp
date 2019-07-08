#include "Submodule_Simulation.h"

#include <sofa/simulation/Simulation.h>
using sofa::simulation::Simulation;

#include <SofaSimulationGraph/DAGSimulation.h>
using sofa::simulation::graph::DAGSimulation;

using sofa::simulation::Node;
using namespace pybind11::literals;

#include <sofa/simulation/Simulation.h>
using sofa::simulation::Simulation;

#include <sofa/core/visual/VisualParams.h>
#include <sofa/core/visual/DrawToolGL.h>

namespace sofapython3
{

PYBIND11_MODULE(Simulation, simulation)
{
    if(!sofa::simulation::getSimulation())
        sofa::simulation::setSimulation(new DAGSimulation());

    simulation.doc() = R"doc(
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
       )doc";

    simulation.def("print", [](Node* n){ sofa::simulation::getSimulation()->print(n); });
    simulation.def("animate", [](Node* n, SReal dt=0.0){ sofa::simulation::getSimulation()->animate(n, dt); });
    simulation.def("init", [](Node* n){ sofa::simulation::getSimulation()->init(n); });
    simulation.def("reset", [](Node* n){ sofa::simulation::getSimulation()->reset(n); });
    simulation.def("load", [](const std::string name){ return sofa::simulation::getSimulation()->load(name.c_str());});
    simulation.def("unload", [](Node* n){ sofa::simulation::getSimulation()->unload(n); });



    simulation.def("updateVisual", [](Node* n){ sofa::simulation::getSimulation()->updateVisual(n); });
    simulation.def("draw", [](Node* n){
        auto* vparam = sofa::core::visual::VisualParams::defaultInstance();
        vparam->drawTool() = new sofa::core::visual::DrawToolGL();
        vparam->setSupported(sofa::core::visual::API_OpenGL);
        sofa::simulation::getSimulation()->draw(vparam, n);
    });

    simulation.def("initTextures", [](Node* n)
    {
        sofa::simulation::getSimulation()->initTextures(n);
    });

    simulation.def("glewInit", []()
    {
        glewInit();
    });
}

} /// namespace sofapython3
