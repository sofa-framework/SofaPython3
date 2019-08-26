#pragma once
#include <functional>
#include <pybind11/pybind11.h>
#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>

#include <sofa/simulation/Node.h>
using sofa::simulation::Node;
using sofa::core::objectmodel::BaseNode;

namespace sofapython3
{
namespace py { using namespace pybind11; }

void moduleAddNode(py::module &m);

} /// namespace sofapython3
