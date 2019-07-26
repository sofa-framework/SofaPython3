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
void registerEvent(std::function<py::object(Event*)> SOFAPYTHON3_API eventBindingFunc, Event* e);

}  // namespace sofapython3
