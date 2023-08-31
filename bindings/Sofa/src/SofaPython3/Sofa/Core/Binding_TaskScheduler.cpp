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

#include <SofaPython3/Sofa/Core/Binding_TaskScheduler.h>
#include <sofa/simulation/MainTaskSchedulerFactory.h>
#include <sofa/simulation/TaskScheduler.h>
#include <SofaPython3/Sofa/Core/Binding_TaskScheduler_doc.h>


/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3
{
py::module moduleAddTaskScheduler(py::module& m)
{
    py::module taskSchedulerModule = m.def_submodule("TaskScheduler");

    taskSchedulerModule.doc() = R"doc(
        TaskScheduler
        -----------------------

        Configuration of the main task scheduler.
    )doc";


    taskSchedulerModule.def("init", [](const unsigned int nbThreads)
    {
        auto* const taskScheduler = sofa::simulation::MainTaskSchedulerFactory::createInRegistry();
        assert(taskScheduler);
        taskScheduler->init(nbThreads);
    }, sofapython3::doc::taskscheduler::init);

    taskSchedulerModule.def("getThreadCount", []()
    {
        const auto* const taskScheduler = sofa::simulation::MainTaskSchedulerFactory::createInRegistry();
        assert(taskScheduler);
        return taskScheduler->getThreadCount();
    }, sofapython3::doc::taskscheduler::getThreadCount);

    taskSchedulerModule.def("stop", []()
    {
        auto* const taskScheduler = sofa::simulation::MainTaskSchedulerFactory::createInRegistry();
        assert(taskScheduler);
        return taskScheduler->stop();
    });

    taskSchedulerModule.def("GetHardwareThreadsCount", []()
    {
        return sofa::simulation::TaskScheduler::GetHardwareThreadsCount();
    }, sofapython3::doc::taskscheduler::GetHardwareThreadsCount);

    return taskSchedulerModule;
}
}
