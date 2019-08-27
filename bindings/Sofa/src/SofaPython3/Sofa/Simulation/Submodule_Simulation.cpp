/*********************************************************************
Copyright 2019, CNRS, University of Lille, INRIA

This file is part of sofaPython3

sofaPython3 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

sofaPython3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
    - bruno.josue.marques@inria.fr
    - eve.le-guillou@centrale.centralelille.fr
    - jean-nicolas.brunet@inria.fr
    - thierry.gaugry@inria.fr
********************************************************************/

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
#include "Submodule_Simulation_doc.h"

namespace sofapython3
{

PYBIND11_MODULE(Simulation, simulation)
{
    if(!sofa::simulation::getSimulation())
        sofa::simulation::setSimulation(new DAGSimulation());

    simulation.doc() =sofapython3::doc::simulation::Class;

    simulation.def("print", [](Node* n){ sofa::simulation::getSimulation()->print(n); }, sofapython3::doc::simulation::print);
    simulation.def("animate", [](Node* n, SReal dt=0.0){ sofa::simulation::getSimulation()->animate(n, dt); },sofapython3::doc::simulation::animate);
    simulation.def("init", [](Node* n){ sofa::simulation::getSimulation()->init(n); }, sofapython3::doc::simulation::init);
    simulation.def("reset", [](Node* n){ sofa::simulation::getSimulation()->reset(n); }, sofapython3::doc::simulation::reset);
    simulation.def("load", [](const std::string name){ return sofa::simulation::getSimulation()->load(name.c_str());}, sofapython3::doc::simulation::load);
    simulation.def("unload", [](Node* n){ sofa::simulation::getSimulation()->unload(n); }, sofapython3::doc::simulation::unload);



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
