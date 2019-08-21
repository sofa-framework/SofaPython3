#include <pybind11/pybind11.h>

#include "Binding_Controller.h"
#include "Binding_Controller_doc.h"

#include <SofaPython3/DataHelper.h>
#include <SofaPython3/PythonFactory.h>


PYBIND11_DECLARE_HOLDER_TYPE(Controller,
                             sofapython3::py_shared_ptr<Controller>, true)

namespace sofapython3
{
    using sofa::core::objectmodel::Event;

    void Controller::init() {
    }

    void Controller::reinit() {
    }

    Controller::Controller() {
    }

    Controller::~Controller() {
    }

    class Controller_Trampoline : public Controller, public PythonTrampoline
    {
    public:
        Controller_Trampoline() = default;

        ~Controller_Trampoline() override = default;

        std::string getClassName() const override
        {
            return pyobject->ob_type->tp_name;
        }

        void init() override ;
        void reinit() override ;
        void handleEvent(Event* event) override ;

    private:
        void callScriptMethod(const py::object& self, Event* event,
                              const std::string & methodName);
    };

    void Controller_Trampoline::init()
    {
        PYBIND11_OVERLOAD(void, Controller, init, );
    }

    void Controller_Trampoline::reinit()
    {
        PYBIND11_OVERLOAD(void, Controller, reinit, );
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

        py::object self = py::cast(this);
        std::string name = std::string("on")+event->getClassName();

        /// Is there a method with this name in the class ?
        if( py::hasattr(self, name.c_str()) )
        {
            py::object fct = self.attr(name.c_str());
            try {
                /// I didn't find any introspection tool in pybind11 to check
                /// if an attr is a callable or not. Using try/catch instead
                ///
                /// Call the matching event in the funcVector & pass it the given event
                callScriptMethod(self, event, name);
                return;
            } catch (std::exception& /*e*/) {
                /// fct is not a method. call the 'onEvent' fallback method instead
            }
        }

        /// Is the fallback method available.
        callScriptMethod(self, event, "onEvent");
    }



    void moduleAddController(py::module &m) {
        py::class_<Controller,
                Controller_Trampoline,
                BaseObject,
                py_shared_ptr<Controller>> f(m, "Controller",
                                             py::dynamic_attr(),
                                             py::multiple_inheritance(),
                                             sofapython3::doc::controller::Controller);

        f.def(py::init([](py::args& /*args*/, py::kwargs& kwargs)
        {
                  auto c = new Controller_Trampoline();
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
