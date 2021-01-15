/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2021 INRIA, USTL, UJF, CNRS, MGH                     *
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

#pragma once

#include <pybind11/pybind11.h>
#include <functional>

#include <sofa/core/objectmodel/Event.h>
using sofa::core::objectmodel::Event;

#include "config.h"

namespace sofapython3 {

namespace py { using namespace pybind11; }

/// Returns the list of methods that will then be called in handleEvent to generate
/// the dictionaries from the Event's values
std::vector<std::function<py::object(Event*)> > SOFAPYTHON3_API getEventDict();

/// Registers a new event type in SofaPython3's event list.
/// \arg eventBindingFunc The (lambda) function taking an event as parameter and
///  returning a py::dict containing the valuable information from the event
/// \arg e The new event to bind
void SOFAPYTHON3_API registerEvent(std::function<py::object(Event*)> eventBindingFunc, Event* e);

}  // namespace sofapython3
