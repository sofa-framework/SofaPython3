#pragma once

#include <pybind11/pybind11.h>

#include <sofa/defaulttype/BoundingBox.h>
using sofa::defaulttype::BoundingBox;

namespace sofapython3 {

namespace py { using namespace pybind11; }

void moduleAddBoundingBox(py::module& m);

}  // namespace sofapython3
