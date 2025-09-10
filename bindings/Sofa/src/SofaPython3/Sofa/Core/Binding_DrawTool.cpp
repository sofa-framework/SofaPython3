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

#include <sofa/core/topology/BaseMeshTopology.h>

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
           for (size_t j = 0; j < 3; ++j)
               points[i][j] = ptr[i * cols + j];

    return points;
}

sofa::type::vector<sofa::type::Quatd> getOrientations(const py::array_t<double>& array)
{
    py::buffer_info buf = array.request();

    if (buf.ndim != 2)
        throw std::runtime_error("Invalid argument, expecting an array with ndim=2");

    size_t rows = buf.shape[0];
    size_t cols = buf.shape[1];

    double* ptr = static_cast<double*>(buf.ptr);

    std::vector<sofa::type::Quatd> orientations;
    orientations.resize(rows);
    for (size_t i = 0; i < rows; ++i)
           for (size_t j = 0; j < cols; ++j)
               orientations[i][j] = ptr[i * cols + j];

    return orientations;
}


void moduleAddDrawTool(py::module &m)
{
    py::class_<DrawTool> dt(m, "DrawTool", sofapython3::doc::drawtool::baseDrawToolClass);

    // Draw points
    dt.def("drawPoints", [](DrawTool *self, py::array_t<double> points, float size, sofa::type::RGBAColor& color)
    {
        self->drawPoints(getPoints(points), size, color);
    }, sofapython3::doc::drawtool::drawPoints);
    dt.def("drawPoints", [](DrawTool *self, BaseData* dpositions, float size, sofa::type::RGBAColor& color){
        auto positions = dynamic_cast<Data<sofa::type::vector<sofa::type::Vec3>>*>(dpositions);
        if(!positions)
            throw std::runtime_error("Invalid argument, a base data of type vector<Vec3> was expected, got "+dpositions->getValueTypeString());

        self->drawPoints(positions->getValue(), size, color);
    });

    /// Draw lines
    dt.def("drawLines", [](DrawTool *self,  const py::array_t<double>& positions, const float size, sofa::type::RGBAColor& color){
        self->drawLines(getPoints(positions), size, color);
    }, sofapython3::doc::drawtool::drawLines);
    dt.def("drawLines", [](DrawTool *self, BaseData* dpositions, const float size, sofa::type::RGBAColor& color){
        auto positions = dynamic_cast<Data<sofa::type::vector<sofa::type::Vec3>>*>(dpositions);
        if(!positions)
            throw std::runtime_error("Invalid argument, expecting a vector<Rigid3> or vector<Vec3>, got "+dpositions->getValueTypeString());

        self->drawLines(positions->getValue(), size, color);
    });

    // Draw disk
    dt.def("drawDisk", [](DrawTool *self,  float radius, double from, double to, int resolution, sofa::type::RGBAColor& color) {
        self->drawDisk(radius, from, to, resolution, color);
    });
    dt.def("drawCircle", [](DrawTool *self,  float radius, float lineThickness, int resolution, sofa::type::RGBAColor& color) {
        self->drawCircle(radius, lineThickness, resolution, color);
    });

    // Draw mesh
    dt.def("drawTriangles", [](DrawTool *self,  py::array_t<double>& positions, sofa::type::RGBAColor& color){
        self->drawTriangles(getPoints(positions), color);
    }, sofapython3::doc::drawtool::drawTriangles);
    dt.def("drawTriangles", [](DrawTool *self,  BaseData* dpositions, BaseData* dtriangles, sofa::type::RGBAColor& color){
        auto positions = dynamic_cast<Data<sofa::type::vector<sofa::type::Vec3d>>*>(dpositions);
        if(!positions)
            throw std::runtime_error("Invalid argument, expecting a vector<Rigid3> or vector<Vec3>, got "+dpositions->getValueTypeString());

        auto triangles = dynamic_cast<Data<sofa::type::vector<sofa::topology::Triangle>>*>(dtriangles);
        if(!triangles)
            throw std::runtime_error("Invalid argument, expecting vector<Triangle>, got "+dtriangles->getValueTypeString());

        auto& cpos = positions->getValue();
        auto& ctris = triangles->getValue();

        std::vector<sofa::type::Vec3> tripos;
        tripos.resize(ctris.size()*3);

        for(auto& ctri : ctris)
        {
            tripos.emplace_back(cpos[ctri[0]]);
            tripos.emplace_back(cpos[ctri[1]]);
            tripos.emplace_back(cpos[ctri[2]]);
        }

        self->drawTriangles(tripos, color);
    });

    // Draw mesh
    dt.def("drawQuads", [](DrawTool *self,  py::array_t<double>& positions, sofa::type::RGBAColor& color){
        self->drawQuads(getPoints(positions), color);
    }, sofapython3::doc::drawtool::drawQuads);
    dt.def("drawQuads", [](DrawTool *self,  BaseData* dpositions, BaseData* dquads, sofa::type::RGBAColor& color){
        auto positions = dynamic_cast<Data<sofa::type::vector<sofa::type::Vec3d>>*>(dpositions);
        if(!positions)
            throw std::runtime_error("Invalid argument, expecting a vector<Rigid3> or vector<Vec3>, got "+dpositions->getValueTypeString());

        auto quads = dynamic_cast<Data<sofa::type::vector<sofa::topology::Quad>>*>(dquads);
        if(!quads)
            throw std::runtime_error("Invalid argument, expecting vector<Quad>, got "+dquads->getValueTypeString());

        auto& cpos = positions->getValue();
        auto& ctris = quads->getValue();

        std::vector<sofa::type::Vec3> quadpos;
        quadpos.resize(ctris.size()*4);

        for(auto& ctri : ctris)
        {
            quadpos.emplace_back(cpos[ctri[0]]);
            quadpos.emplace_back(cpos[ctri[1]]);
            quadpos.emplace_back(cpos[ctri[2]]);
            quadpos.emplace_back(cpos[ctri[3]]);
        }

        self->drawQuads(quadpos, color);
    });


    // Draw spheres
    dt.def("drawSpheres", [](DrawTool *self,  const py::array_t<double>& positions, const std::vector<float>& radius, sofa::type::RGBAColor& color){
        self->drawSpheres(getPoints(positions), radius, color);
    }, sofapython3::doc::drawtool::drawSpheres);
    dt.def("drawSpheres", [](DrawTool *self, BaseData* dpositions, const float radius, sofa::type::RGBAColor& color){
        auto positions = dynamic_cast<Data<sofa::type::vector<sofa::type::Vec3>>*>(dpositions);
        if(!positions)
            throw std::runtime_error("Invalid argument, expecting a vector<Rigid3> or vector<Vec3>, got "+dpositions->getValueTypeString());
        self->drawSpheres(positions->getValue(), radius, color);
    });

    // Draw boundingBox
    dt.def("drawBoundingBox", [](DrawTool *self,  const std::array<double,4>& min, const std::array<double, 4>& max, double width){
        sofa::type::Vec3d cmin { min[0], min[1], min[2] };
        sofa::type::Vec3d cmax { max[0], max[1], max[2] };
        self->drawBoundingBox( cmin, cmax, width);
    }, sofapython3::doc::drawtool::drawBoundingBox);

    // Draw frames
    dt.def("drawFrames", [](DrawTool* self,
           const py::array_t<double>& points,
           const py::array_t<double>& orientations,
           const std::array<double,3>& size){
        auto cpoints = getPoints(points);
        auto corientations = getOrientations(orientations);
        sofa::type::Vec3 csize {size[0],size[1],size[2]};
        for(unsigned int i=0;i<cpoints.size();i++)
        {
            self->drawFrame(cpoints[i], corientations[i], csize);
        }
    }, sofapython3::doc::drawtool::drawFrames);
    dt.def("drawFrames", [](DrawTool* self, BaseData* dpositions, std::array<double, 3>& size ){
        using sofa::defaulttype::Rigid3Types;
        using Coord = sofa::defaulttype::Rigid3Types::Coord;
        auto positions = dynamic_cast<Data<sofa::type::vector<Coord>>*>(dpositions);
        sofa::type::Vec3 csize {size[0],size[1],size[2]};
        if(!positions)
            throw std::runtime_error("Invalid argument");

        for(auto& position : positions->getValue())
        {
            self->drawFrame(Rigid3Types::getCPos(position),
                            Rigid3Types::getCRot(position), csize);
        }
    });

    dt.def("enableLighting", [](DrawTool* self){ self->enableLighting(); });
    dt.def("disableLighting", [](DrawTool* self){ self->disableLighting(); });

    // Draw text
    dt.def("drawText", [](DrawTool* self,
                          const std::array<double,3>& point,
                          const float size,
                          const std::string& text,
                          const sofa::type::RGBAColor& color)
    {
        self->draw3DText(point, size, color, text.c_str());
    }, sofapython3::doc::drawtool::drawText);

    // Draw overlay text
    dt.def("drawOverlayText", [](DrawTool* self, const std::array<double,2>& point,
                                 int fontSize, char* text, sofa::type::RGBAColor& color){
        self->writeOverlayText(point[0],point[1], fontSize, color, text);
    }, sofapython3::doc::drawtool::drawOverlayText);

}

} /// namespace sofapython3
