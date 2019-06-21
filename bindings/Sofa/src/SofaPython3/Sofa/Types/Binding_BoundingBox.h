#ifndef PYTHONMODULE_SOFA_BINDING_BOUNDINGBOX_H
#define PYTHONMODULE_SOFA_BINDING_BOUNDINGBOX_H

#include <pybind11/pybind11.h>

#include <sofa/defaulttype/BoundingBox.h>
using sofa::defaulttype::BoundingBox;

namespace sofapython3 {

namespace py { using namespace pybind11; }

void moduleAddBoundingBox(py::module& m);

}  // namespace sofapython3


#endif  // PYTHONMODULE_SOFA_BINDING_BOUNDINGBOX_H
