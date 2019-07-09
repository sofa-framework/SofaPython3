#include <sofa/core/objectmodel/Data.h>
#include "Binding_BoundingBox.h"
#include "SofaPython3/DataHelper.h"

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


    std::cout << "ADDING BOUNDINGBOX" << std::endl;
    getBindingDataFactoryInstance()->registerCreator(
                "BoundingBox", new TypeCreator<sofa::Data<sofa::defaulttype::BoundingBox>*>());
    std::cout << "ADDING BOUNDINGBOX" << std::endl;


}

}  // namespace sofapython3

