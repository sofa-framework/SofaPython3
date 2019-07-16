#pragma once
#include <pybind11/pybind11.h>

namespace sofapython3
{
namespace py { using namespace pybind11; }

void moduleAddSystem(py::module &m);

} /// namespace sofapython3

