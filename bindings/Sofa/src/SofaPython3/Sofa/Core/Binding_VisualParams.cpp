/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2021 INRIA, USTL, UJF, CNRS, MGH                     *
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

#include <sofa/type/Quat.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <sofa/core/visual/VisualParams.h>

#include <SofaPython3/Sofa/Core/Binding_VisualParams.h>
#include <SofaPython3/Sofa/Core/Binding_VisualParams_doc.h>

#include <SofaPython3/PythonFactory.h>
#include <sofa/core/objectmodel/Data.h>
#include <sofa/type/RGBAColor.h>

namespace py { using namespace pybind11; }
using sofa::core::visual::VisualParams;

namespace sofapython3 {

void moduleAddVisualParams(py::module &m)
{
    py::class_<VisualParams> vp(m, "VisualParams", sofapython3::doc::visualparams::baseVisualParamsClass);

    vp.def("getDrawTool", [](VisualParams *self){ return self->drawTool(); },
           pybind11::return_value_policy::reference);
}

} /// namespace sofapython3
