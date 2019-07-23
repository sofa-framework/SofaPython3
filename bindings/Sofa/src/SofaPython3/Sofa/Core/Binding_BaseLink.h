#pragma once

#include <sofa/core/objectmodel/BaseLink.h>
#include "Binding_Base.h"
using sofa::core::objectmodel::BaseLink;
#include <pybind11/pybind11.h>

namespace py { using namespace pybind11; }

namespace sofapython3
{
    void moduleAddBaseLink(py::module& m);

} /// sofapython3
