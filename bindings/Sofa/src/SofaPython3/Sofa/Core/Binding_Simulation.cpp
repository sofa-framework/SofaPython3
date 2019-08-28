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

#include "Binding_Simulation.h"

#include <sofa/simulation/Simulation.h>
using sofa::simulation::Simulation;

#include <SofaSimulationGraph/DAGSimulation.h>
using sofa::simulation::graph::DAGSimulation;

using sofa::simulation::Node;
using namespace pybind11::literals;

namespace sofapython3
{

void moduleAddSimulation(py::module &m)
{
  if(!sofa::simulation::getSimulation())
    sofa::simulation::setSimulation(new DAGSimulation());

  py::module singleRuntime = m.def_submodule("Simulation");
  singleRuntime.def("print", [](Node* n){ sofa::simulation::getSimulation()->print(n); });
  singleRuntime.def("animate", [](Node* n, SReal dt=0.0){ sofa::simulation::getSimulation()->animate(n, dt); });
  singleRuntime.def("init", [](Node* n){ sofa::simulation::getSimulation()->init(n); });
  singleRuntime.def("reset", [](Node* n){ sofa::simulation::getSimulation()->reset(n); });
  singleRuntime.def("load", [](const std::string name){ return sofa::simulation::getSimulation()->load(name.c_str());});
  singleRuntime.def("unload", [](Node* n){ sofa::simulation::getSimulation()->unload(n); });
}


} /// namespace sofapython3
