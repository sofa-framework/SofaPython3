#pragma once

#include <pybind11/pybind11.h>
#include <sofa/core/objectmodel/BaseData.h>

namespace sofapython3
{

namespace py { using namespace pybind11; }
using sofa::core::objectmodel::BaseData;

void moduleAddDataLink(py::module& m);

class DataLink : public BaseData
{
public:
    py::str __str__();
    py::str __repr__();
    py::str getTargetPath();
    py::object getTargetBase();
    void setTargetPath(const std::string& targetPath);
};

}  // namespace sofapython3
