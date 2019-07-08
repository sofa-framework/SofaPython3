#pragma once

#include <pybind11/pybind11.h>

namespace sofapython3 {
/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }
// @TODO: Remove with pybind11 > 2.2.4
namespace py {
PYBIND11_RUNTIME_EXCEPTION(attribute_error, PyExc_AttributeError)
}

void moduleAddGuiManager(py::module& m);

} /// namespace sofapython3