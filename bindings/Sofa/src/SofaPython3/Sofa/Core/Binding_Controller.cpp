#include <pybind11/pybind11.h>
#include <pybind11/detail/init.h>

#include "Binding_Base.h"
#include "Binding_Controller.h"
#include "DataHelper.h"

#include <sofa/core/objectmodel/Event.h>
using sofa::core::objectmodel::Event;


#include "sofa/simulation/AnimateBeginEvent.h"
using sofa::simulation::AnimateBeginEvent;
#include "sofa/simulation/AnimateEndEvent.h"
using sofa::simulation::AnimateEndEvent;
//#include "sofa/simulation/CollisionBeginEvent.h"
//#include "sofa/simulation/CollisionEndEvent.h"
//#include "sofa/simulation/IntegrateBeginEvent.h"
//#include "sofa/simulation/IntegrateEndEvent.h"
//#include "sofa/simulation/PauseEvent.h"
//#include "sofa/simulation/PositionEvent.h"
//#include "sofa/simulation/UpdateMappingEndEvent.h"

//#include "sofa/core/objectmodel/DetachNodeEvent.h"
//#include "sofa/core/objectmodel/GUIEvent.h"
//#include "sofa/core/objectmodel/HapticDeviceEvent.h"
//#include "sofa/core/objectmodel/IdleEvent.h"
//#include "sofa/core/objectmodel/JoystickEvent.h"
#include "sofa/core/objectmodel/KeypressedEvent.h"
using sofa::core::objectmodel::KeypressedEvent;
#include "sofa/core/objectmodel/KeyreleasedEvent.h"
using sofa::core::objectmodel::KeyreleasedEvent;
#include "sofa/core/objectmodel/MouseEvent.h"
using sofa::core::objectmodel::MouseEvent;
#include "sofa/core/objectmodel/ScriptEvent.h"
using sofa::core::objectmodel::ScriptEvent;

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

    private:
        void callScriptMethod(const py::object& self, Event* event,
                              std::string methodName);
        void initEventDict(std::vector<std::function<py::object(Event*)> >&);
        static std::vector<std::function<py::object(Event*)> > s_getEventDict;
        static bool s_isDictCreated;

    };

    bool Controller_Trampoline::s_isDictCreated = false;
    std::vector<std::function<py::object(Event*)> > Controller_Trampoline::s_getEventDict = std::vector<std::function<py::object(Event*)> >();

    void Controller_Trampoline::init()
    {
        PYBIND11_OVERLOAD(void, Controller, init, );
    }

    void Controller_Trampoline::reinit()
    {
        PYBIND11_OVERLOAD(void, Controller, reinit, );
    }


    /// Creates the methods that will then be called in handleEvent to generate
    /// the dictionaries from the Event's values
    void Controller_Trampoline::initEventDict(
                std::vector<std::function<py::object(Event*)> >& eventDict)
    {
        /// kind of ugly -> not choice but to instantiate a dummy typed event
        /// to get the event's uid
        Event* e = new AnimateBeginEvent(.0);
        eventDict[e->getEventTypeIndex()] = [] (Event* event) -> py::object {
            AnimateBeginEvent* evt = dynamic_cast<AnimateBeginEvent*>(event);
            return py::dict("type"_a=evt->getClassName(),
                            "isHandled"_a=evt->isHandled(),
                            "dt"_a=evt->getDt());
        };
        e = new AnimateEndEvent(.0);
        eventDict[e->getEventTypeIndex()] = [] (Event* event) -> py::object {
            AnimateEndEvent* evt = dynamic_cast<AnimateEndEvent*>(event);
            return py::dict("type"_a=evt->getClassName(),
                            "isHandled"_a=evt->isHandled(),
                            "dt"_a=evt->getDt());
        };

        e = new KeypressedEvent('\0');
        eventDict[e->getEventTypeIndex()] = [] (Event* event) -> py::object {
            KeypressedEvent* evt = dynamic_cast<KeypressedEvent*>(event);
            return py::dict("type"_a=evt->getClassName(),
                            "isHandled"_a=evt->isHandled(),
                            "key"_a=evt->getKey());
        };
        e = new KeyreleasedEvent('\0');
        eventDict[e->getEventTypeIndex()] = [] (Event* event) -> py::object {
            KeyreleasedEvent* evt = dynamic_cast<KeyreleasedEvent*>(event);
            return py::dict("type"_a=evt->getClassName(),
                            "isHandled"_a=evt->isHandled(),
                            "key"_a=evt->getKey());
        };

        e = new MouseEvent(MouseEvent::State::Move);
        eventDict[e->getEventTypeIndex()] = [] (Event* event) -> py::object {
            MouseEvent* evt = dynamic_cast<MouseEvent*>(event);

            return py::dict("type"_a=evt->getClassName(),
                            "isHandled"_a=evt->isHandled(),
                            "State"_a=int(evt->getState()),
                            "mouseX"_a=evt->getPosX(),
                            "mouseY"_a=evt->getPosY(),
                            "wheelDelta"_a=evt->getWheelDelta());
        };

        e = new ScriptEvent(nullptr, "");
        eventDict[e->getEventTypeIndex()] = [] (Event* event) -> py::object {
            ScriptEvent* evt = dynamic_cast<ScriptEvent*>(event);
            return py::dict("type"_a=evt->getClassName(),
                            "isHandled"_a=evt->isHandled(),
                            "sender"_a=py::cast(evt->getSender()),
                            "event_name"_a=evt->getEventName());
        };

        // TODO: bind other events' attributes here
    }

    /// If a method named "methodName" exists in the python controller,
    /// methodName is called, with the Event's dict as argument
    void Controller_Trampoline::callScriptMethod(
                const py::object& self, Event* event, std::string methodName)
    {
        if( py::hasattr(self, methodName.c_str()) )
        {
            py::object fct = self.attr(methodName.c_str());

            if (s_getEventDict.at(event->getEventTypeIndex()) != nullptr)
                fct(s_getEventDict[event->getEventTypeIndex()](event));
            else
                fct(py::dict("type"_a=event->getClassName(),
                             "isHandled"_a=event->isHandled()));
        }
    }

    void Controller_Trampoline::handleEvent(Event* event)
    {
        if (!s_isDictCreated)
        {
            s_getEventDict.resize(sofa::core::objectmodel::Event::getEventTypeCount() + 1, nullptr);
            initEventDict(s_getEventDict);
            s_isDictCreated = true;
        }

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
