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

using sofa::type::BoundingBox;
using sofa::core::objectmodel::BaseData;

namespace py { using namespace pybind11; }

namespace sofapython3 {

void moduleAddBoundingBox(py::module& m)
{
    py::class_<sofa::Data<BoundingBox>, sofa::core::objectmodel::BaseData,
        std::unique_ptr<sofa::Data<BoundingBox>, pybind11::nodelete> > bbox(m, "BoundingBox");
    bbox.def("getMin", [](sofa::Data<BoundingBox>& bbox) {
        py::list list;
        list.append(bbox.getValue().minBBox()[0]);
        list.append(bbox.getValue().minBBox()[1]);
        list.append(bbox.getValue().minBBox()[2]);
        return list;
    }, "Returns a list (by copy) of the min bbox position");
    bbox.def("setMin", [](sofa::Data<BoundingBox>& bbox, py::list val) {
        BoundingBox& bb = *bbox.beginEdit();
        bb.minBBox()[0] = py::cast<double>(val[0]);
        bb.minBBox()[1] = py::cast<double>(val[1]);
        bb.minBBox()[2] = py::cast<double>(val[2]);
        bbox.endEdit();
    }, "sets the min bbox position from a python list");
    bbox.def("getMax", [](sofa::Data<BoundingBox>& bbox) {
        py::list list;
        list.append(bbox.getValue().maxBBox()[0]);
        list.append(bbox.getValue().maxBBox()[1]);
        list.append(bbox.getValue().maxBBox()[2]);
        return list;
    }, "Returns a list (by copy) of the max bbox position");
    bbox.def("setMax", [](sofa::Data<BoundingBox>& bbox, py::list val) {
        BoundingBox& bb = *bbox.beginEdit();
        bb.maxBBox()[0] = py::cast<double>(val[0]);
        bb.maxBBox()[1] = py::cast<double>(val[1]);
        bb.maxBBox()[2] = py::cast<double>(val[2]);
        bbox.endEdit();
    }, "sets the max bbox position from a python list");

    bbox.def("getCenter", [](sofa::Data<BoundingBox>& bbox) {
        const auto val = bbox.getValue().maxBBox() - bbox.getValue().maxBBox();
        py::list list;
        list.append(val[0]);
        list.append(val[1]);
        list.append(val[2]);
        return list;
    }, "returns the center of the bbox");

    PythonFactory::registerType("BoundingBox", [](BaseData* data) -> py::object {
        // Weird bug here, using a dynamic_cast will fail to convert the BaseData to a Data<BoundingBox> on MacOS and
        // MSVC, even if the type is good. We therefore use in reinterpret_cast to force it until we understand why
        // the initial cast fails.
        auto bbox = reinterpret_cast<sofa::Data<BoundingBox>*>(data);
        return py::cast(bbox);
    });

}

}  // namespace sofapython3

