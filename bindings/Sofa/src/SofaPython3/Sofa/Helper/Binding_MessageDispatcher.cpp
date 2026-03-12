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
#include <SofaPython3/Sofa/Helper/Binding_MessageDispatcher.h>
#include <sofa/helper/logging/MessageDispatcher.h>

namespace py { using namespace pybind11; }

void sofapython3::moduleAddMessageDispatcher(pybind11::module &m)
{
    py::module messageDispatcherModule = m.def_submodule("MessageDispatcher");

    messageDispatcherModule.doc() = R"doc(
        MessageDispatcher
        -----------------------

        Configuration of the message dispatcher.
    )doc";

    messageDispatcherModule.def("clearHandlers",
        [](){
            sofa::helper::logging::MessageDispatcher::clearHandlers();
        }, "Removes all registered message handlers.");

    messageDispatcherModule.def("numHandlers",
        [](){
            return sofa::helper::logging::MessageDispatcher::getHandlers().size();
        }, "Returns the number of registered message handlers.");
}
