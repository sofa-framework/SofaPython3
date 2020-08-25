#pragma once

#include <pybind11/pybind11.h>
#include <sofa/core/objectmodel/BaseData.h>

namespace sofapython3
{

namespace py { using namespace pybind11; }
using sofa::core::objectmodel::BaseData;

void moduleAddDataLink(py::module& m);

}  // namespace sofapython3
