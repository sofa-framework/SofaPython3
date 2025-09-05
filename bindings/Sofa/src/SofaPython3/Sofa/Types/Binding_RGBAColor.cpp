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

#include <SofaPython3/Sofa/Types/Binding_BoundingBox.h>

#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/core/objectmodel/Data.h>
#include <sofa/type/BoundingBox.h>
#include <SofaPython3/PythonFactory.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
using sofa::type::RGBAColor;

namespace py { using namespace pybind11; }

namespace sofapython3 {

void moduleAddRGBAColor(py::module& m)
{
    py::class_<sofa::type::RGBAColor> c(m, "RGBAColor");
    c.doc() = R"doc(A color)doc";

    c.def(py::init<>());                  // empty ctor
    c.def(py::init<const RGBAColor &>()); // copy ctor
    c.def(py::init([](double r, double g, double b, double a) {
              return std::make_unique<sofa::type::RGBAColor>( r,g,b,a );
          }));

    c.def(py::init([](std::array<double,4>& v) {
              return std::make_unique<sofa::type::RGBAColor>( v[0], v[1], v[2], v[3] );
          }));

    c.def("r", [](const RGBAColor& color) { return color.r(); });
    c.def("g", [](const RGBAColor& color) { return color.g(); });
    c.def("b", [](const RGBAColor& color) { return color.b(); });
    c.def("a", [](const RGBAColor& color) { return color.a(); });

    c.def("lighten", [](const RGBAColor& color, const SReal factor){
        return RGBAColor::lighten(color, factor);
    });

    c.def("__str__", [](const RGBAColor& color){
        std::stringstream tmp;
        tmp << "RGBAColor(" << color.r() << "," << color.g() << "," << color.b() << "," << color.a() << ")";
        return tmp.str();
    });

    c.def("__repr__", [](const RGBAColor& color){
        std::stringstream tmp;
        tmp << "RGBAColor(" << color.r() << "," << color.g() << "," << color.b() << "," << color.a() << ")";
        return tmp.str();
    });

    // Dark magic to define comparison operator using the c++ == and != operator.
    c.def(pybind11::self==pybind11::self);
    c.def(pybind11::self!=pybind11::self);
}

}  // namespace sofapython3

