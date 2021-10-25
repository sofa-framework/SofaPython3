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

#include <pybind11/eval.h>
namespace py = pybind11;

#include <SofaExporter/Binding_STLExporter.h>
#include <SofaExporter/Binding_OBJExporter.h>

namespace sofapython3
{

PYBIND11_MODULE(SofaExporter, m) {
    m.doc() = R"doc(
              Binding for the SofaExporter plugin
              -----------------------------------

              Provides python bindings for the SofaExporter module

              Example of use:

              .. code-block:: python

                import SofaExporter

              .. autosummary::
                  :toctree: _autosummary/_autosummary

                  SofaExporter.STLExporter
                  SofaExporter.OBJExporter

              )doc";

    py::module::import("Sofa.Core");

    moduleAddSTLExporter(m);
    moduleAddOBJExporter(m);
}

}  // namespace sofapython3

