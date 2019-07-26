#include "Binding_PythonScriptEvent.h"

#include <SofaPython3/EventFactory.h>

namespace sofapython3 {
using namespace pybind11::literals;

PythonScriptEvent::PythonScriptEvent(sofa::simulation::Node::SPtr sender, const char* eventName, py::object* userData)
    : sofa::core::objectmodel::ScriptEvent(sender,eventName)
    , m_userData(userData){}

PythonScriptEvent::~PythonScriptEvent(){}

void moduleAddPythonScriptEvent(py::module& /*module*/)
{
    PythonScriptEvent pse(nullptr, "", nullptr);
    registerEvent([] (Event* event) -> py::object {
        auto evt = dynamic_cast<PythonScriptEvent*>(event);
        return py::dict("type"_a=evt->getClassName(),
                        "isHandled"_a=evt->isHandled(),
                        "sender"_a=py::cast(evt->getSender()),
                        "event_name"_a=evt->getEventName(),
                        "userData"_a=evt->getUserData());
    }, &pse);

}

}  // namespace sofapython3

