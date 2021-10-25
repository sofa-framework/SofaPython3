/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2021 CNRS, University of Lille, INRIA                  *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaExporter/Binding_OBJExporter.h>
#include <SofaExporter/Binding_OBJExporter_doc.h>

#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>
#include <SofaExporter/OBJExporter.h>

using  sofa::component::exporter::OBJExporter;

namespace py { using namespace pybind11; }

namespace sofapython3 {

void moduleAddOBJExporter(py::module &m)
{
    PythonFactory::registerType<OBJExporter>([](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<OBJExporter*>(object));
    });

    py::class_<OBJExporter, sofa::core::objectmodel::BaseObject, py_shared_ptr<OBJExporter>> p(m, "OBJExporter");

    p.def("write", &OBJExporter::write, sofapython3::doc::SofaExporter::OBJExporter::write::docstring);
}

} // namespace sofapython3
