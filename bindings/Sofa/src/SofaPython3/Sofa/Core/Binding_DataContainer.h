#pragma once

#include <pybind11/pybind11.h>
#include <sofa/core/objectmodel/BaseData.h>

namespace sofapython3
{
/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }
using sofa::core::objectmodel::BaseData;

void moduleAddDataContainer(py::module& m);
void moduleAddDataAsString(py::module& m);
void moduleAddWriteAccessor(py::module& m);

class DataContainer : public BaseData {} ;
class DataAsString : public BaseData {} ;

class WriteAccessor
{
public:
    WriteAccessor(BaseData* data_, py::object fct_) : data(data_), fct(fct_){}

    BaseData* data {nullptr};
    py::object wrap;
    py::object fct;
};

}
