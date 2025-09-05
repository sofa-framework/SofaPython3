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

#include <SofaPython3/Sofa/Core/Binding_DrawTool.h>
#include <SofaPython3/Sofa/Core/Binding_DrawTool_doc.h>

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

sofa::type::vector<sofa::type::Vec3> getPoints(const py::array_t<double>& array)
{
    py::buffer_info buf = array.request();

    if (buf.ndim != 2)
        throw std::runtime_error("Invalid argument, expecting an array with ndim=2");

    size_t rows = buf.shape[0];
    size_t cols = buf.shape[1];

    double* ptr = static_cast<double*>(buf.ptr);

    std::vector<sofa::type::Vec3d> points;
    points.resize(rows);
    for (size_t i = 0; i < rows; ++i)
           for (size_t j = 0; j < cols; ++j)
               points[i][j] = ptr[i * cols + j];

    return points;
}


void moduleAddDrawTool(py::module &m)
{
    py::class_<DrawTool> dt(m, "DrawTool", sofapython3::doc::drawtool::baseDrawToolClass);

    // Draw points from vectors...
    dt.def("drawPoints", [](DrawTool *self, py::array_t<double> points, float size, sofa::type::RGBAColor& color)
    {
        self->drawPoints(getPoints(points), size, color);
    });

    // Draw points from a base data that can be casted to a vector of Vec3
    dt.def("drawPoints", [](DrawTool *self, BaseData* dpositions, float size ){
        auto positions = dynamic_cast<Data<sofa::type::vector<sofa::type::Vec3>>*>(dpositions);
        if(!positions)
            throw std::runtime_error("Invalid argument, a base data of type vector<Vec3> was expected, got "+dpositions->getValueTypeString());

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
    dt.def("drawText", [](DrawTool* self,
                            const sofa::type::Vec3d& point,
                            const float size,
                            const std::string& text)
    {
        self->draw3DText(point, size, sofa::type::RGBAColor::white(), text.c_str());
    });

    dt.def("drawOverlayText", [](DrawTool* self, int x, int y, int fontSize, char* text){
        self->writeOverlayText(x,y, fontSize, sofa::type::RGBAColor::white(), text);
    });
}

} /// namespace sofapython3
