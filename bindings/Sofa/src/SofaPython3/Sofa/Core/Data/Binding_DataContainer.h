#pragma once

#include <pybind11/pybind11.h>
#include <sofa/core/objectmodel/BaseData.h>

namespace sofapython3
{
/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }
using sofa::core::objectmodel::BaseData;

void moduleAddDataContainer(py::module& m);
void moduleAddDataContainerContext(py::module& m);


class DataContainer : public BaseData {
public:
    py::tuple getShape();
    size_t getNDim();
    size_t getSize();
} ;

/// @brief A python context manager to control writeable state of a DataContainer.
///
/// This context manager handle in a graceful way the writeable state of a DataContainer
/// so the Data counter gets updated correctly when we left the context.
///
/// It allows to implement stuff like that:
///   with myobject.position.arrayContext() as wa:
///         wa[3] = [1,2,3]
///         wa[4] = ...
///  # When we leave the context the data counter gets updated
///
/// NB: context is a classic python idom defined in
///    https://docs.python.org/3/reference/datamodel.html#context-managers
///
class DataContainerContext
{
public:
    DataContainerContext(BaseData* data_, py::object fct_) : data(data_), fct(fct_){}

    BaseData* data {nullptr};
    py::object wrap;
    py::object fct;
};

}
