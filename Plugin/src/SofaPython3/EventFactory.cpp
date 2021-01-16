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

#include "EventFactory.h"

#include "sofa/simulation/AnimateBeginEvent.h"
using sofa::simulation::AnimateBeginEvent;
#include "sofa/simulation/AnimateEndEvent.h"
using sofa::simulation::AnimateEndEvent;
//#include "sofa/simulation/CollisionBeginEvent.h"
//#include "sofa/simulation/CollisionEndEvent.h"
//#include "sofa/simulation/IntegrateBeginEvent.h"
//#include "sofa/simulation/IntegrateEndEvent.h"
//#include "sofa/simulation/PauseEvent.h"
//#include "sofa/simulation/PositionEvent.h"
//#include "sofa/simulation/UpdateMappingEndEvent.h"

//#include "sofa/core/objectmodel/DetachNodeEvent.h"
//#include "sofa/core/objectmodel/GUIEvent.h"
//#include "sofa/core/objectmodel/HapticDeviceEvent.h"
//#include "sofa/core/objectmodel/IdleEvent.h"
//#include "sofa/core/objectmodel/JoystickEvent.h"
#include "sofa/core/objectmodel/KeypressedEvent.h"
using sofa::core::objectmodel::KeypressedEvent;
#include "sofa/core/objectmodel/KeyreleasedEvent.h"
using sofa::core::objectmodel::KeyreleasedEvent;
#include "sofa/core/objectmodel/MouseEvent.h"
using sofa::core::objectmodel::MouseEvent;
#include "sofa/core/objectmodel/ScriptEvent.h"
using sofa::core::objectmodel::ScriptEvent;
#include <sofa/core/objectmodel/Event.h>
using sofa::core::objectmodel::Event;

namespace sofapython3 {

using namespace pybind11::literals;

std::vector<std::function<py::object(Event*)> > getEventDict()
{
    static std::vector<std::function<py::object(Event*)> > s_eventDict;

    if (s_eventDict.empty())
    {
        s_eventDict.resize(sofa::core::objectmodel::Event::getEventTypeCount() + 1, nullptr);
        AnimateBeginEvent abe(.0);
        s_eventDict[abe.getEventTypeIndex()] = [] (Event* event) -> py::object {
            auto evt = dynamic_cast<AnimateBeginEvent*>(event);
            return py::dict("type"_a=evt->getClassName(),
                            "isHandled"_a=evt->isHandled(),
                            "dt"_a=evt->getDt());
        };
        AnimateEndEvent aee(.0);
        s_eventDict[aee.getEventTypeIndex()] = [] (Event* event) -> py::object {
            auto evt = dynamic_cast<AnimateEndEvent*>(event);
            return py::dict("type"_a=evt->getClassName(),
                            "isHandled"_a=evt->isHandled(),
                            "dt"_a=evt->getDt());
        };

        KeypressedEvent kpe('\0');
        s_eventDict[kpe.getEventTypeIndex()] = [] (Event* event) -> py::object {
            auto evt = dynamic_cast<KeypressedEvent*>(event);
            return py::dict("type"_a=evt->getClassName(),
                            "isHandled"_a=evt->isHandled(),
                            "key"_a=evt->getKey());
        };
        KeyreleasedEvent kre('\0');
        s_eventDict[kre.getEventTypeIndex()] = [] (Event* event) -> py::object {
            auto evt = dynamic_cast<KeyreleasedEvent*>(event);
            return py::dict("type"_a=evt->getClassName(),
                            "isHandled"_a=evt->isHandled(),
                            "key"_a=evt->getKey());
        };

        MouseEvent me(MouseEvent::State::Move);
        s_eventDict[me.getEventTypeIndex()] = [] (Event* event) -> py::object {
            auto evt = dynamic_cast<MouseEvent*>(event);

            return py::dict("type"_a=evt->getClassName(),
                            "isHandled"_a=evt->isHandled(),
                            "State"_a=int(evt->getState()),
                            "mouseX"_a=evt->getPosX(),
                            "mouseY"_a=evt->getPosY(),
                            "wheelDelta"_a=evt->getWheelDelta());
        };

        ScriptEvent se(nullptr, "");
        s_eventDict[se.getEventTypeIndex()] = [] (Event* event) -> py::object {
            auto evt = dynamic_cast<ScriptEvent*>(event);
            return py::dict("type"_a=evt->getClassName(),
                            "isHandled"_a=evt->isHandled(),
                            "sender"_a=(evt->getSender() ? py::cast(evt->getSender()) : py::none()),
                            "event_name"_a=evt->getEventName());
        };

        // TODO: bind other events' attributes here
    }
    return s_eventDict;
}

void registerEvent(std::function<py::object(Event*)> eventBindingFunc, Event* e)
{
    getEventDict().resize(sofa::core::objectmodel::Event::getEventTypeCount() + 1, nullptr);
    getEventDict()[e->getEventTypeIndex()] = eventBindingFunc;
}

}  // namespace sofapython3
