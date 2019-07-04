#pragma once

#include <pybind11/pybind11.h>
namespace py = pybind11;
using namespace pybind11::literals;


#include <sofa/defaulttype/Quat.h>
using sofa::defaulttype::Quat;

void moduleAddQuat(py::module& m);
