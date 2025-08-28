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
using sofa::core::objectmodel::BaseData;
using sofa::core::objectmodel::Data;
using sofa::core::objectmodel::BaseObject;
using sofa::core::visual::VisualParams;
using sofa::core::visual::DrawTool;

namespace sofapython3 {

void moduleAddVisualParams(py::module &m)
{
    py::class_<VisualParams> vp(m, "VisualParams", sofapython3::doc::visualParams::baseVisualParamsClass);
    vp.def("getDrawTool", [](VisualParams *self){ return self->drawTool(); },
           pybind11::return_value_policy::reference);

    py::class_<DrawTool> dt(m, "DrawTool", sofapython3::doc::visualParams::baseVisualParamsClass);
    dt.def("drawPoints", [](DrawTool *self, const std::vector<sofa::type::Vec3> &points, float size ){
        self->drawPoints(points, size, sofa::type::RGBAColor::white());
    });
    dt.def("drawPoints", [](DrawTool *self, BaseData* dpositions, float size ){
        auto positions = dynamic_cast<Data<sofa::type::vector<sofa::type::Vec3>>*>(dpositions);
        if(!positions)
            throw std::runtime_error("Invalid argument");

        self->drawPoints(positions->getValue(), size, sofa::type::RGBAColor::white());
    });
    dt.def("drawLines", [](DrawTool *self, const std::vector<sofa::type::Vec3> &points, float size ){
        self->drawLines(points, size, sofa::type::RGBAColor::white());
    });
    dt.def("drawFrames", [](DrawTool* self,
           const std::vector<sofa::type::Vec3d>& points,
           const std::vector<sofa::type::Quatd>& orientations,
           const sofa::type::Vec3& size ){
        for(unsigned int i=0;i<points.size();i++)
        {
            self->drawFrame(points[i], orientations[i], size);
        }
    });
    dt.def("drawFrames", [](DrawTool* self, BaseData* dpositions, const sofa::type::Vec3& size ){
        using sofa::defaulttype::Rigid3Types;
        using Coord = sofa::defaulttype::Rigid3Types::Coord;
        auto positions = dynamic_cast<Data<sofa::type::vector<Coord>>*>(dpositions);
        if(!positions)
            throw std::runtime_error("Invalid argument");

        for(auto& position : positions->getValue())
        {
            self->drawFrame(Rigid3Types::getCPos(position),
                            Rigid3Types::getCRot(position), size);
        }
    });
    dt.def("draw3DText", [](DrawTool* self,
                            const sofa::type::Vec3d& point,
                            const float size,
                            const std::string& text)
    {
        self->draw3DText(point, size, sofa::type::RGBAColor::white(), text.c_str());
    });
}

} /// namespace sofapython3
