#include "Submodule_Simulation.h"

#include <sofa/simulation/Simulation.h>
using sofa::simulation::Simulation;

#include <SofaSimulationGraph/DAGSimulation.h>
using sofa::simulation::graph::DAGSimulation;

using sofa::simulation::Node;
using namespace pybind11::literals;

#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>

#include <sofa/simulation/Simulation.h>
using sofa::simulation::Simulation;

template class pybind11::class_<Simulation, Simulation::SPtr>;


namespace sofapython3
{

py::module addSubmoduleSimulation(py::module &m)
{
    if(!sofa::simulation::getSimulation())
        sofa::simulation::setSimulation(new DAGSimulation());

    py::module simulation = m.def_submodule("Simulation");

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

    return simulation;
}

} /// namespace sofapython3
