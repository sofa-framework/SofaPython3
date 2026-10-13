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
#include <pybind11/cast.h>
#include <sofa/core/visual/VisualParams.h>
#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_Controller.h>
#include <SofaPython3/Sofa/Core/Binding_Component.inl>
#include <SofaPython3/Sofa/Core/Binding_Controller_doc.h>
#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/PythonEnvironment.h>

SOFAPYTHON3_BIND_ATTRIBUTE_ERROR()

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3
{
using sofa::core::objectmodel::Event;
using sofa::core::behavior::BaseController;

// ---------------------------------------------------------------------------
// Controller_Trampoline
// ---------------------------------------------------------------------------

Controller_Trampoline::Controller_Trampoline()
    : BasetTrampoline(this)   // pass this as BaseComponent* — no CRTP needed
{
}

void Controller_Trampoline::draw(const sofa::core::visual::VisualParams* params)
{
    PythonEnvironment::executePython(this, [this, params](){
        PYBIND11_OVERLOAD(void, Controller, draw, params);
    });
}

void Controller_Trampoline::init()
{
    PythonEnvironment::executePython(this, [this](){
        initializePythonCache();
        PYBIND11_OVERLOAD(void, Controller, init, );
    });
}

void Controller_Trampoline::reinit()
{
    PythonEnvironment::executePython(this, [this](){
        PYBIND11_OVERLOAD(void, Controller, reinit, );
    });
}

void Controller_Trampoline::handleEvent(sofa::core::objectmodel::Event* event)
{
    trampoline_handleEvent(event);
}

std::string Controller_Trampoline::getClassName() const
{
    return trampoline_getClassName();
}

// ---------------------------------------------------------------------------
// Module registration
// ---------------------------------------------------------------------------

void moduleAddController(py::module &m) {
    py::class_<Controller,
                Controller_Trampoline,
                BaseComponent,
                py_shared_ptr<Controller>> f(m, "Controller",
                                             py::dynamic_attr(),
                                             sofapython3::doc::controller::controllerClass);

    f.def(py::init(&trampoline_init<Controller_Trampoline>));
    f.def("__setattr__", &trampoline_setattr<Controller_Trampoline>);

    f.def("init", &Controller::init);
    f.def("reinit", &Controller::reinit);
    f.def("draw", [](Controller& self, sofa::core::visual::VisualParams* params){
        self.draw(params);
    }, pybind11::return_value_policy::reference);
}

} // namespace sofapython3
