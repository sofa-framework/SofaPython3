#pragma once

#include <pybind11/pybind11.h>

#include <SofaExporter/STLExporter.h>
#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>

template class pybind11::class_<sofa::component::exporter::STLExporter,
                                sofa::core::objectmodel::BaseObject,
                                sofa::core::sptr<sofa::component::exporter::STLExporter>>;


namespace sofapython3
{

void moduleAddSTLExporter(py::module &m);

} /// namespace sofapython3
