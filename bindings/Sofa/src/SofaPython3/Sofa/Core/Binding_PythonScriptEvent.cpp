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

#include <SofaPython3/Sofa/Core/Binding_PythonScriptEvent.h>
#include <SofaPython3/PythonFactory.h>
#include <sofa/core/objectmodel/Event.h>
#include <sofa/simulation/Node.h>

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3 {
using namespace pybind11::literals;

SOFA_EVENT_CPP(PythonScriptEvent)

PythonScriptEvent::PythonScriptEvent(sofa::core::sptr<sofa::simulation::Node> sender, const char* eventName, py::object userData)
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

