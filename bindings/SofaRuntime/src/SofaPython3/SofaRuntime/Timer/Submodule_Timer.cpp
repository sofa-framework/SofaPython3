#include <sofa/helper/AdvancedTimer.h>
using sofa::helper::AdvancedTimer;

#include "Submodule_Timer.h"



namespace sofapython3
{

py::module addSubmoduleTimer(py::module &m)
{

    py::module timer = m.def_submodule("Timer");

    /// TODO: fill the docstring!
    timer.doc() = R"doc(
           Timer
           -----------------------
           Advanced timer, meant to gather precise statistics for results in published papers.
           Not so advanced for now, but it will be...
       )doc";

    timer.def("clear", AdvancedTimer::clear, "Clears the AdvancedTimer");

    timer.def("isEnabled", &AdvancedTimer::isEnabled, py::arg("id"),
              "Returns True if the given id is enabled, False otherwise");
    timer.def("setEnabled", &AdvancedTimer::setEnabled, py::arg("id"), py::arg("enabled"),
              "enables or disables the given timer");

    timer.def("getInterval", &AdvancedTimer::getInterval, py::arg("id"), "returns the Timer's interval");
    timer.def("setInterval", &AdvancedTimer::setInterval, py::arg("id"), py::arg("interval"),
              "sets the interval for the given timer");

    timer.def("begin", [](const std::string& id){ AdvancedTimer::begin(id);}, py::arg("id"));
    timer.def("stepBegin", [](const std::string& id){ AdvancedTimer::stepBegin(id);}, py::arg("id"));
    timer.def("stepEnd", [](const std::string& id){ AdvancedTimer::stepEnd(id);}, py::arg("id"));
    timer.def("end", [](const std::string& id){ AdvancedTimer::end(id);}, py::arg("id"));

    timer.def("setOutputType", &AdvancedTimer::setOutputType, py::arg("id"), py::arg("newOutputType"),
              "Changes the output type for a given timer");

    return timer;
}

} /// namespace sofapython3
