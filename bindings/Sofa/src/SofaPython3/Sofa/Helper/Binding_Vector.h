#ifndef SOFAPYTHON3_SOFA_HELPER_BINDING_VECTOR_H
#define SOFAPYTHON3_SOFA_HELPER_BINDING_VECTOR_H

#include <pybind11/pybind11.h>

namespace sofapython3
{

/// Makes an alias for the pybind11 namespace to increase readability.
    namespace py { using namespace pybind11; }

    void moduleAddVector(py::module &m);

} ///sofapython3


#endif //SOFAPYTHON3_SOFA_HELPER_BINDING_VECTOR_H
