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

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_BaseContext.h>
#include <SofaPython3/PythonFactory.h>
#include <sofa/core/BaseState.h>
#include <sofa/core/behavior/BaseMechanicalState.h>
#include <sofa/core/topology/Topology.h>
#include <sofa/core/topology/BaseMeshTopology.h>
#include <sofa/core/behavior/BaseMass.h>

namespace py { using namespace pybind11; }

using namespace sofa::core::objectmodel;
namespace sofapython3 {

void moduleAddBaseContext(py::module& m) {
    py::class_<BaseContext, Base, py_shared_ptr<BaseContext>> c (m, "BaseContext");

    py::enum_<BaseContext::SearchDirection>(c, "SearchDirection")
        .value("SearchUp",      BaseContext::SearchDirection::SearchUp)
        .value("Local",         BaseContext::SearchDirection::Local)
        .value("SearchDown",    BaseContext::SearchDirection::SearchDown)
        .value("SearchRoot",    BaseContext::SearchDirection::SearchRoot)
        .value("SearchParents", BaseContext::SearchDirection::SearchParents)
        .export_values();

    // Getters
    c.def("isActive",   &BaseContext::isActive, "The Context is active");
    c.def("isSleeping", &BaseContext::isSleeping, "Sleeping state of the context");
    c.def("canChangeSleepingState", &BaseContext::canChangeSleepingState, "Whether the context can change its sleeping state or not");
    c.def("getTime", &BaseContext::getTime, "Simulation time");
    c.def("getDt",   &BaseContext::getDt, "Simulation timestep");
    c.def("getAnimate", &BaseContext::getAnimate, "Animation flag");
    c.def("getGravity", &BaseContext::getGravity, "Gravity in local coordinates");
    c.def("getRootContext", &BaseContext::getRootContext, "Get the root context of the graph");
    c.def("getState", &BaseContext::getState, "Mechanical Degrees-of-Freedom");
    c.def("getMechanicalState", &BaseContext::getMechanicalState, "Mechanical Degrees-of-Freedom");
    c.def("getTopology", &BaseContext::getTopology, "Topology");
    c.def("getMeshTopology", &BaseContext::getMeshTopology, "Mesh Topology (unified interface for both static and dynamic topologies)");
    c.def("getMeshTopologyLink", &BaseContext::getMeshTopologyLink, "Mesh Topology (unified interface for both static and dynamic topologies)");
    c.def("getMass", &BaseContext::getMass, "Mass");

    c.def("__str__", [](const BaseContext & context) {std::ostringstream s; s << context; return s.str();}, "Get a string representation of the context.");

    // Setters
    c.def("setActive", &BaseContext::setActive, "State of the context");
    c.def("setGravity", &BaseContext::setGravity, "Gravity in local coordinates");
    c.def("setDt", &BaseContext::setDt, "Simulation timestep");
    c.def("setAnimate", &BaseContext::setAnimate, "Animation flag");
    c.def("setSleeping", &BaseContext::setSleeping, "Sleeping state of the context");
    c.def("setChangeSleepingState", &BaseContext::setChangeSleepingState, "Sleeping state change of the context");
}

} // namespace sofapython3
