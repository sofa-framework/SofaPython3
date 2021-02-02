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
using sofa::simulation::Simulation;

#include <SofaSimulationGraph/DAGSimulation.h>
using sofa::simulation::graph::DAGSimulation;

#include <sofa/simulation/Node.h>
using sofa::simulation::Node;
using namespace pybind11::literals;

#include <sofa/simulation/Simulation.h>
using sofa::simulation::Simulation;

#include <sofa/core/visual/VisualParams.h>
#include <SofaPython3/Sofa/Simulation/Submodule_Simulation_doc.h>

#include <sofa/core/init.h>
#include <sofa/simulation/init.h>
#include <SofaSimulationGraph/init.h>

namespace py = pybind11;

namespace sofapython3
{

PYBIND11_MODULE(Simulation, simulation)
{
    // These are needed to force the dynamic loading of module dependencies (found in CMakeLists.txt)
    sofa::core::init();
    sofa::simulation::core::init();
    sofa::simulation::graph::init();

    if(!sofa::simulation::getSimulation())
        sofa::simulation::setSimulation(new DAGSimulation());

    simulation.doc() =sofapython3::doc::simulation::Class;

    simulation.def("print", [](Node* n){ sofa::simulation::getSimulation()->print(n); }, sofapython3::doc::simulation::print);
    simulation.def("animate", [](Node* n, SReal dt=0.0){ sofa::simulation::getSimulation()->animate(n, dt); },sofapython3::doc::simulation::animate);
    simulation.def("init", [](Node* n){ sofa::simulation::getSimulation()->init(n); }, sofapython3::doc::simulation::init);
    simulation.def("initVisual", [](Node* n){ n->getVisualLoop()->initStep(sofa::core::visual::VisualParams::defaultInstance()); });
    simulation.def("reset", [](Node* n){ sofa::simulation::getSimulation()->reset(n); }, sofapython3::doc::simulation::reset);
    simulation.def("load", [](const std::string & name) {
        sofa::simulation::Node::SPtr node = sofa::simulation::getSimulation()->load(name);
        return node ? py::cast(node.get()) : py::none();
    }, sofapython3::doc::simulation::load);
    simulation.def("unload", [](Node* n){ sofa::simulation::getSimulation()->unload(n); }, sofapython3::doc::simulation::unload);
    simulation.def("updateVisual", [](Node* n){ sofa::simulation::getSimulation()->updateVisual(n); });
    simulation.def("initTextures", [](Node* n)
    {
        sofa::simulation::getSimulation()->initTextures(n);
    });
}

} /// namespace sofapython3
