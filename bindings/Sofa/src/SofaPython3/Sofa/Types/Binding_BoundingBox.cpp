/*********************************************************************
Copyright 2019, CNRS, University of Lille, INRIA

This file is part of sofaPython3

sofaPython3 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

sofaPython3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
    - bruno.josue.marques@inria.fr
    - eve.le-guillou@centrale.centralelille.fr
    - jean-nicolas.brunet@inria.fr
    - thierry.gaugry@inria.fr
********************************************************************/

#include "Binding_BoundingBox.h"

#include <sofa/core/objectmodel/Data.h>
#include <SofaPython3/DataHelper.h>
#include <SofaPython3/PythonFactory.h>

namespace sofapython3 {

void moduleAddBoundingBox(py::module& m)
{
    py::class_<sofa::Data<BoundingBox>> bbox(m, "BoundingBox");
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
        const sofa::defaulttype::Vec3 val = bbox.getValue().maxBBox() - bbox.getValue().maxBBox();
        py::list list;
        list.append(val[0]);
        list.append(val[1]);
        list.append(val[2]);
        return list;
    }, "returns the center of the bbox");

    std::cout << "Lets register BoundingBox" << std::endl;

    PythonFactory::registerType<sofa::Data<BoundingBox>>("BoundingBox", [](BaseData* data) -> py::object {
        return py::cast(reinterpret_cast<sofa::Data<BoundingBox>*>(data));
    });

}

}  // namespace sofapython3

