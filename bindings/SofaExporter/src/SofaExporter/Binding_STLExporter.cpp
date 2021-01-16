/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2019 CNRS, University of Lille, INRIA                  *
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
* Authors: see Authors.txt                                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
    - bruno.josue.marques@inria.fr
    - eve.le-guillou@centrale.centralelille.fr
    - jean-nicolas.brunet@inria.fr
    - thierry.gaugry@inria.fr
********************************************************************/

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaExporter/Binding_STLExporter.h>
#include <SofaExporter/Binding_STLExporter_doc.h>

#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>
#include <SofaExporter/STLExporter.h>

using  sofa::component::exporter::STLExporter;

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3 {

void moduleAddSTLExporter(py::module &m)
{
    PythonFactory::registerType<STLExporter>(
                [](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<STLExporter*>(object));
    });

    py::class_<STLExporter, sofa::core::objectmodel::BaseObject, py_shared_ptr<STLExporter>> p(m, "STLExporter");

    p.def("write", &STLExporter::write, sofapython3::doc::SofaExporter::STLExporter::write::docstring);
}

} // namespace sofapython3
