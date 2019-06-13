#include <pybind11/pybind11.h>
#include <pybind11/detail/init.h>

#include "Binding_Base.h"
#include "Binding_Controller.h"
#include "DataHelper.h"

#include <sofa/core/objectmodel/Event.h>
using sofa::core::objectmodel::Event;

PYBIND11_DECLARE_HOLDER_TYPE(Controller,
                             sofapython3::py_shared_ptr<Controller>, true)

namespace sofapython3
{

    void Controller::init() {
    }

    void Controller::reinit() {
    }

    class Controller_Trampoline : public Controller, public PythonTrampoline
    {
    public:
        Controller_Trampoline()
        {
        }

        ~Controller_Trampoline() override
        {
        }

        virtual std::string getClassName() const override
        {
            return pyobject->ob_type->tp_name;
        }

        virtual void init() override ;
        virtual void reinit() override ;
        virtual void handleEvent(Event* event) override ;
    };

    void Controller_Trampoline::init()
    {
        PYBIND11_OVERLOAD(void, Controller, init, );
    }

    void Controller_Trampoline::reinit()
    {
        PYBIND11_OVERLOAD(void, Controller, reinit, );
    }

    void Controller_Trampoline::handleEvent(Event* event)
    {
        py::object self = py::cast(this);
        std::string name = std::string("on")+event->getClassName();

        /// Is there a method with this name in the class ?
        if( py::hasattr(self, name.c_str()) )
        {
            py::object fct = self.attr(name.c_str());
            try {
                /// I didn't find any introspection tool in pybind11 to check
                /// if an attr is a callable or not. Using try/catch instead
                py::object ret = fct(name);
                return;
            } catch (std::exception& /*e*/) {
                /// fct is not a method. call the 'onEvent' fallback method instead
            }
        }

        /// Is the fallback method available.
        if( py::hasattr(self, "onEvent") )
        {
            py::object fct = self.attr("onEvent");
            py::object ret = fct(name);
        }
    }

    void moduleAddController(py::module &m) {
        py::class_<Controller,
                Controller_Trampoline,
                BaseObject,
                py_shared_ptr<Controller>> f(m, "Controller",
                                             py::dynamic_attr(),
                                             py::multiple_inheritance());

        f.def(py::init([](py::args& /*args*/, py::kwargs& kwargs)
        {
                  Controller_Trampoline* c = new Controller_Trampoline();
                  c->f_listening.setValue(true);

                  for(auto kv : kwargs)
                  {
                      std::string key = py::cast<std::string>(kv.first);
                      py::object value = py::object(kv.second, true);

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
