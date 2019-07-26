#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/detail/init.h>
#include <sofa/core/objectmodel/ScriptEvent.h>

#include <SofaPython3/config.h>

namespace sofapython3
{
namespace py { using namespace pybind11; }

class SOFAPYTHON3_API PythonScriptEvent : public sofa::core::objectmodel::ScriptEvent
{
public:
    SOFAPYTHON3_API PythonScriptEvent(sofa::simulation::Node::SPtr sender, const char* eventName, py::object* userData=nullptr);
    SOFAPYTHON3_API ~PythonScriptEvent() override;
    py::object* SOFAPYTHON3_API getUserData() const { return m_userData; }
    const char* getClassName() const override { return "PythonScriptEvent"; }

private:
    py::object* m_userData;
};

void moduleAddPythonScriptEvent();

} // namespace objectmodel

