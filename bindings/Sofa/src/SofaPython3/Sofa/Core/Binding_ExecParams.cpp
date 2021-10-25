/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2021 CNRS, University of Lille, INRIA                  *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include <SofaPython3/Sofa/Core/Binding_ExecParams.h>
#include <sofa/core/ExecParams.h>

namespace py { using namespace pybind11; }

namespace sofapython3 {

void moduleAddExecParams(py::module& m) {
    using namespace sofa::core;
    py::class_<ExecParams> c (m, "ExecParams", "Class gathering parameters use by most components methods, and transmitted by all visitors");
    c.def(py::init());

    py::enum_<ExecParams::ExecMode> (c, "ExecMode")
        .value("EXEC_NONE", ExecParams::ExecMode::EXEC_NONE)
        .value("EXEC_DEFAULT", ExecParams::ExecMode::EXEC_DEFAULT)
        .value("EXEC_DEBUG", ExecParams::ExecMode::EXEC_DEBUG)
        .value("EXEC_GPU", ExecParams::ExecMode::EXEC_GPU)
        .value("EXEC_GRAPH", ExecParams::ExecMode::EXEC_GRAPH)
        .export_values()
    ;

    // Public properties/methods
    c.def("checkValidStorage", &ExecParams::checkValidStorage);
    c.def("execMode", &ExecParams::execMode, "Mode of execution requested");
    c.def("threadID", &ExecParams::threadID, "Index of current thread (0 corresponding to the only thread in sequential mode, or first thread in parallel mode)");
    c.def("nbThreads", &ExecParams::nbThreads, "Number of threads currently known to Sofa");
    c.def("update", &ExecParams::update, "Make sure this instance is up-to-date relative to the current thread");
    c.def("setExecMode", &ExecParams::setExecMode, "Request a specific mode of execution");
    c.def("setThreadID", &ExecParams::setThreadID, "Specify the index of the current thread");

}

} // namespace sofapython3
