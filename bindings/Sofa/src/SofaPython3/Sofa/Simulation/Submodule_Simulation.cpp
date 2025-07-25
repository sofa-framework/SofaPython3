/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2021 CNRS, University of Lille, INRIA                  *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include <pybind11/pybind11.h>

#include <sofa/simulation/Simulation.h>
#include <sofa/simulation/Node.h>
#include <memory>
#include "SofaPython3/DataHelper.h"
using sofa::simulation::Simulation;

#include <sofa/simulation/graph/DAGSimulation.h>
using sofa::simulation::graph::DAGSimulation;

#include <sofa/simulation/Node.h>
using sofa::simulation::Node;
using namespace pybind11::literals;

#include <sofa/simulation/Simulation.h>
using sofa::simulation::Simulation;

#include <sofa/core/visual/VisualParams.h>
#include <sofa/core/visual/VisualLoop.h>
#include <SofaPython3/Sofa/Simulation/Submodule_Simulation_doc.h>
#include <SofaPython3/Sofa/Simulation/Binding_SceneCheck.h>
#include <SofaPython3/Sofa/Simulation/Binding_SceneCheckMainRegistry.h>

#include <sofa/core/init.h>
#include <sofa/simulation/init.h>
#include <sofa/simulation/common/init.h>
#include <sofa/simulation/graph/init.h>

namespace py = pybind11;

namespace sofapython3
{

PYBIND11_MODULE(Simulation, simulation)
{
    // These are needed to force the dynamic loading of module dependencies (found in CMakeLists.txt)
    sofa::core::init();
    sofa::simulation::core::init();
    sofa::simulation::graph::init();

    simulation.doc() =sofapython3::doc::simulation::Class;

    moduleAddSceneCheck(simulation);
    moduleAddSceneCheckMainRegistry(simulation);

    simulation.def("print", [](Node* n){ sofa::simulation::node::print(n); }, sofapython3::doc::simulation::print);
    simulation.def("animate", [](Node* n, SReal dt=0.0){ sofa::simulation::node::animate(n, dt); },sofapython3::doc::simulation::animate);
    simulation.def("init", [](Node* n){ sofa::simulation::node::init(n); }, sofapython3::doc::simulation::init);
    simulation.def("initRoot", [](Node* n){ sofa::simulation::node::initRoot(n); }, sofapython3::doc::simulation::initRoot);
    simulation.def("initVisual", [](Node* n){ n->getVisualLoop()->initStep(sofa::core::visual::VisualParams::defaultInstance()); }, sofapython3::doc::simulation::initVisual);
    simulation.def("reset", [](Node* n){ sofa::simulation::node::reset(n); }, sofapython3::doc::simulation::reset);
  
    simulation.def("load", [](const std::string & name)
    {
        sofa::simulation::Node::SPtr node = sofa::simulation::node::load(name);
        return node ? py::cast(node.get()) : py::none();
    }, sofapython3::doc::simulation::load);
  
    simulation.def("unload", [](Node* n)
    {
            auto& memcache = getObjectCache();
            sofa::simulation::node::unload(n);
            memcache.clear();
    }, sofapython3::doc::simulation::unload);
  
    simulation.def("animateNSteps", [](Node *n, int n_steps, SReal dt=0.0)
    {
        for (int i = 0; i < n_steps; i++)
        {
            sofa::simulation::node::animate(n, dt); //Execute one timestep. If dt is 0, the dt parameter in the graph will be used
        }
    }, sofapython3::doc::simulation::animateNSteps, py::arg("root_node"), py::arg("n_steps") = 1, py::arg("dt") = 0.0);
  
    simulation.def("updateVisual", [](Node* n)
    {
        sofa::simulation::node::updateVisual(n);
    }, sofapython3::doc::simulation::updateVisual);

    simulation.def("initTextures", [](Node* n)
    {
        sofa::simulation::node::initTextures(n);
    });

    // called when the module is unloaded
    auto atexit = py::module_::import("atexit");
    atexit.attr("register")(py::cpp_function([]() {

        sofa::simulation::core::cleanup();
        sofa::simulation::common::cleanup();
        sofa::simulation::graph::cleanup();

        msg_info("SofaPython3.Simulation") << "Sofa.Simulation unload()";
    }));
}

} /// namespace sofapython3
