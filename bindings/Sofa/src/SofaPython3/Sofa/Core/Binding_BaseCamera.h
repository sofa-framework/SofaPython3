#pragma once

#include "Binding_BaseObject.h"
#include <SofaBaseVisual/BaseCamera.h>

template class pybind11::class_<sofa::component::visualmodel::BaseCamera,
                                sofa::core::objectmodel::BaseObject,
                                sofa::core::sptr<sofa::component::visualmodel::BaseCamera>>;

namespace sofapython3
{

using sofa::component::visualmodel::BaseCamera;
void moduleAddBaseCamera(py::module &m);
} /// namespace sofapython3

