#pragma once

#include <pybind11/numpy.h>

////////////////////////// FORWARD DECLARATION ///////////////////////////
namespace sofa {
    namespace core {
        namespace objectmodel {
            class BaseData;
        }
    }
}


/////////////////////////////// DECLARATION //////////////////////////////
namespace sofapython3
{

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

using sofa::core::objectmodel::BaseData;

///@brief Search if there is already a numpy array associated with the BaseData
bool hasArrayFor(BaseData* d);

///@brief
py::array PYBIND11_EXPORT resetArrayFor(BaseData* d);
py::array PYBIND11_EXPORT getPythonArrayFor(BaseData* d);
void trimCache();

} /// sofapython3

