#include "Binding_PythonScriptEvent.h"

#include <SofaPython3/PythonFactory.h>
#include <sofa/core/objectmodel/Event.h>

namespace sofapython3 {
using namespace pybind11::literals;

SOFA_EVENT_CPP(PythonScriptEvent)

PythonScriptEvent::PythonScriptEvent(sofa::simulation::Node::SPtr sender, const char* eventName, py::object userData)
    : sofa::core::objectmodel::ScriptEvent(sender,eventName)
    , m_userData(userData){}

PythonScriptEvent::~PythonScriptEvent(){}

void moduleAddPythonScriptEvent()
{
    PythonFactory::registerType<PythonScriptEvent>(
                [] (sofa::core::objectmodel::Event* event) -> py::dict {
        auto evt = dynamic_cast<PythonScriptEvent*>(event);

        py::dict d("type"_a=evt->getClassName(),
                   "isHandled"_a=evt->isHandled(),
                   "sender"_a=(evt->getSender().get() ? py::cast(evt->getSender().get()) : py::none()),
                   "event_name"_a=py::cast(evt->getEventName()),
                   "userData"_a=evt->getUserData()
                   );
        return d;
    });
}

}  // namespace sofapython3

