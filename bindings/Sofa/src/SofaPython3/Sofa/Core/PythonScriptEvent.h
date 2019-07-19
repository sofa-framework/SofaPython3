#pragma once

#include <pybind11/pybind11.h>
#include <sofa/core/objectmodel/ScriptEvent.h>

namespace sofapython3
{
namespace py { using namespace pybind11; }


class PythonScriptEvent : public sofa::core::objectmodel::ScriptEvent
{
public:
    PythonScriptEvent(sofa::simulation::Node::SPtr sender, const char* eventName, py::object* userData=NULL);

    ~PythonScriptEvent() override;
    py::object* getUserData() const {return m_userData;}
    const char* getClassName() const override { return "PythonScriptEvent"; }

private:

    py::object* m_userData;

};

} // namespace objectmodel

