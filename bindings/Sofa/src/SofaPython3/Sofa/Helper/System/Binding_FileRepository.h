#pragma once
#include <pybind11/pybind11.h>

namespace sofapython3
{

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

void moduleAddFileRepository(py::module &m);

} ///sofapython3