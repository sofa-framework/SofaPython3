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

#include <SofaPython3/Sofa/Core/Binding_Component.inl>

SOFAPYTHON3_BIND_ATTRIBUTE_ERROR()

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3
{
using sofa::core::objectmodel::Event;
using sofa::core::objectmodel::BaseComponent;

// ---------------------------------------------------------------------------
// Component_Trampoline
// ---------------------------------------------------------------------------

Component_Trampoline::Component_Trampoline()
    : TrampolineBase(this)   // pass this as BaseComponent* — no CRTP needed
{
}

void Component_Trampoline::draw(const sofa::core::visual::VisualParams* params)
{
    PythonEnvironment::executePython(this, [this, params](){
        PYBIND11_OVERLOAD(void, Component, draw, params);
    });
}

void Component_Trampoline::init()
{
    PythonEnvironment::executePython(this, [this](){
        initializePythonCache();
        PYBIND11_OVERLOAD(void, Component, init, );
    });
}

void Component_Trampoline::reinit()
{
    PythonEnvironment::executePython(this, [this](){
        PYBIND11_OVERLOAD(void, Component, reinit, );
    });
}

void Component_Trampoline::handleEvent(sofa::core::objectmodel::Event* event)
{
    trampoline_handleEvent(event);
}

std::string Component_Trampoline::getClassName() const
{
    return trampoline_getClassName();
}

// ---------------------------------------------------------------------------
// Module registration
// ---------------------------------------------------------------------------

void moduleAddComponent(py::module &m) {
    py::class_<Component,
        Component_Trampoline,
        BaseComponent,
        py_shared_ptr<Component>> f(m, "Component",
                                     py::dynamic_attr(),
                                     sofapython3::doc::component::componentClass);

    f.def(py::init(&trampoline_init<Component_Trampoline>));
    f.def("__setattr__", &trampoline_setattr<Component_Trampoline>);

    f.def("init", &Component::init);
    f.def("reinit", &Component::reinit);
    f.def("draw", [](Component& self, sofa::core::visual::VisualParams* params){
        self.draw(params);
    }, pybind11::return_value_policy::reference);
}

} // namespace sofapython3
