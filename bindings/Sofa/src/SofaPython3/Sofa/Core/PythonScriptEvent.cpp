#include "PythonScriptEvent.h"

namespace sofapython3 {

PythonScriptEvent::PythonScriptEvent(sofa::simulation::Node::SPtr sender, const char* eventName, py::object* userData)
    : sofa::core::objectmodel::ScriptEvent(sender,eventName)
    , m_userData(userData){}
PythonScriptEvent::~PythonScriptEvent(){}
}
