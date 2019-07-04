#pragma once

#include "Binding_BaseObject.h"

#include <sofa/core/behavior/BaseController.h>

template class pybind11::class_<sofa::core::behavior::BaseController,
                          sofa::core::objectmodel::BaseObject,
                          sofa::core::sptr<sofa::core::behavior::BaseController>>;


namespace sofapython3
{
using sofa::core::behavior::BaseController;

void moduleAddBaseController(py::module &m);

} /// namespace sofapython3

