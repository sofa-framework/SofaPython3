#include <pybind11/pybind11.h>
#include <pybind11/detail/init.h>

#include "Binding_Base.h"
#include "Binding_BaseController.h"
#include <SofaPython3/DataHelper.h>

#include <sofa/core/objectmodel/Event.h>
using sofa::core::objectmodel::Event;

PYBIND11_DECLARE_HOLDER_TYPE(BaseController,
                             sofapython3::py_shared_ptr<BaseController>, true)

namespace sofapython3
{

    class BaseController_Trampoline : public BaseController, public PythonTrampoline
    {
    public:
        BaseController_Trampoline()
        {
        }

        ~BaseController_Trampoline() override
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

    void BaseController_Trampoline::init()
    {
        PYBIND11_OVERLOAD(void, BaseController, init, );
    }

    void BaseController_Trampoline::reinit()
    {
        PYBIND11_OVERLOAD(void, BaseController, reinit, );
    }

    void BaseController_Trampoline::handleEvent(Event* event)
    {
        py::object self = py::cast(this);
        std::string name = std::string("on")+event->getClassName();

        /// Is there a method with this name in the class ?
        if( py::hasattr(self, name.c_str()) )
        {
            py::object fct = self.attr(name.c_str());
            py::object ret = fct(name);
            return;
        }

        /// Is the fallback method available.
        if( py::hasattr(self, "onEvent") )
        {
            py::object fct = self.attr("onEvent");
            py::object ret = fct(name);
        }
    }

    void moduleAddBaseController(py::module &m) {
        py::class_<BaseController,
                BaseController_Trampoline,
                BaseObject,
                py_shared_ptr<BaseController>> f(m, "BaseController",
                                                   py::dynamic_attr(),
                                                   py::multiple_inheritance());

        f.def(py::init([](py::args& args, py::kwargs& kwargs)
        {
                  BaseController_Trampoline* c = new BaseController_Trampoline();
                  c->f_listening.setValue(true);

                  if(args.size() != 0)
                  {
                      if(args.size()==1) c->setName(py::cast<std::string>(args[0]));
                      else throw py::type_error("Only one un-named arguments can be provided.");
                  }

                  for(auto kv : kwargs)
                  {
                      std::string key = py::cast<std::string>(kv.first);
                      py::object value = py::reinterpret_borrow<py::object>(kv.second);
                      if( key == "name")
                      {
                          if( args.size() != 0 )
                          {
                              throw py::type_error("The name is setted twice as a "
                              "named argument='"+py::cast<std::string>(value)+"' and as a"
                              "positional argument='"+py::cast<std::string>(args[0])+"'.");
                          }
                      }
                      BindingBase::SetAttr(*c, key, value);
                  }

                  return c;
              }));
    }


}
