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

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_Controller.h>
#include <SofaPython3/Sofa/Core/Binding_Controller_doc.h>

#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/PythonEnvironment.h>

/// Bind the python's attribute error
namespace pybind11 { PYBIND11_RUNTIME_EXCEPTION(attribute_error, PyExc_AttributeError) }
/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3
{
using sofa::core::objectmodel::Event;
using sofa::core::objectmodel::BaseObject;

std::string Controller_Trampoline::getClassName() const
{
    PythonEnvironment::gil acquire {"getClassName"};
    // Get the actual class name from python.
    return py::str(py::cast(this).get_type().attr("__name__"));
}

void Controller_Trampoline::init()
{
    PythonEnvironment::executePython(this, [this](){
        PYBIND11_OVERLOAD(void, Controller, init, );
    });
}

void Controller_Trampoline::reinit()
{
    PythonEnvironment::executePython(this, [this](){
        PYBIND11_OVERLOAD(void, Controller, reinit, );
    });
}

/// If a method named "methodName" exists in the python controller,
/// methodName is called, with the Event's dict as argument
void Controller_Trampoline::callScriptMethod(
        const py::object& self, Event* event, const std::string & methodName)
{
    if( py::hasattr(self, methodName.c_str()) )
    {
        py::object fct = self.attr(methodName.c_str());
        fct(PythonFactory::toPython(event));
    }
}

void Controller_Trampoline::handleEvent(Event* event)
{
    PythonEnvironment::executePython(this, [this,event](){
        py::object self = py::cast(this);
        std::string name = std::string("on")+event->getClassName();
        /// Is there a method with this name in the class ?
        if( py::hasattr(self, name.c_str()) )
        {
            py::object fct = self.attr(name.c_str());
            if (PyCallable_Check(fct.ptr())) {
                callScriptMethod(self, event, name);
                return;
            }
        }

        /// Is the fallback method available.
        callScriptMethod(self, event, "onEvent");
    });
}

void moduleAddController(py::module &m) {
    py::class_<Controller,
            Controller_Trampoline,
            BaseObject,
            py_shared_ptr<Controller>> f(m, "Controller",
                                         py::dynamic_attr(),
                                         sofapython3::doc::controller::Controller);

    f.def(py::init([](py::args& /*args*/, py::kwargs& kwargs)
          {
              auto c = sofa::core::sptr<Controller_Trampoline> (new Controller_Trampoline());
              c->f_listening.setValue(true);

              for(auto kv : kwargs)
              {
                  std::string key = py::cast<std::string>(kv.first);
                  py::object value = py::reinterpret_borrow<py::object>(kv.second);

                  if( key == "name")
                  c->setName(py::cast<std::string>(kv.second));
                  try {
                      BindingBase::SetAttr(*c, key, value);
                  } catch (py::attribute_error& /*e*/) {
                      /// kwargs are used to set datafields to their initial values,
                      /// but they can also be used as simple python attributes, unrelated to SOFA.
                      /// thus we catch & ignore the py::attribute_error thrown by SetAttr
                  }
              }
              return c;
          }));

    f.def("init", &Controller::init);
    f.def("reinit", &Controller::reinit);
}


}
