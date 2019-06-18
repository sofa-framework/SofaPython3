#include "Binding_Vector.h"

#include <sofa/helper/vector.h>
#include <sofa/core/objectmodel/BaseData.h>

#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py { using namespace pybind11; }


/**
 * To add sofa::helper::vector types, follow the 2 steps below
 */

// ------------------------------------
// STEP 1: Make the std vector type opaque
// ------------------------------------
PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(std::vector<long>);
PYBIND11_MAKE_OPAQUE(std::vector<unsigned int>);
PYBIND11_MAKE_OPAQUE(std::vector<unsigned long>);
PYBIND11_MAKE_OPAQUE(std::vector<float>);
PYBIND11_MAKE_OPAQUE(std::vector<double>);
PYBIND11_MAKE_OPAQUE(std::vector<sofa::core::objectmodel::BaseData*>);

template<typename T>
void declareVector(py::module &m, const std::string &typestr) {
    std::string std_pyclass_name = std::string("StdVector") + typestr;
    auto v = py::bind_vector<std::vector<T>>(m, std_pyclass_name);

    std::string pyclass_name = std::string("Vector") + typestr;

    py::class_<sofa::helper::vector<T>> (m, pyclass_name.c_str(), v);
}

namespace sofapython3 {

void moduleAddVector(py::module &m) {
// ------------------------------------
// STEP 2: Declare the vector type to python
// ------------------------------------
    declareVector<int>(m, "int");
    declareVector<long>(m, "long");
    declareVector<unsigned int>(m, "uint");
    declareVector<unsigned long>(m, "ulong");
    declareVector<float>(m, "float");
    declareVector<double>(m, "double");
    declareVector<sofa::core::objectmodel::BaseData*>(m, "BaseData");
}

} /// namespace sofapython3