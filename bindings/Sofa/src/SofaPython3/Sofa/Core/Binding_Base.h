#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseData.h>

/// More info about smart pointer in
/// /pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html
PYBIND11_DECLARE_HOLDER_TYPE(Base, sofa::core::sptr<Base>, true)

template class pybind11::class_<sofa::core::objectmodel::Base,
                                sofa::core::objectmodel::Base::SPtr>;

namespace sofapython3
{
/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }
// @TODO: Remove with pybind11 > 2.2.4
namespace py {
    PYBIND11_RUNTIME_EXCEPTION(attribute_error, PyExc_AttributeError)
}

using namespace pybind11::literals;
using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseData;
using sofa::core::sptr;

class BindingBase
{
public:
    static void SetAttr(Base& self, const std::string& s, py::object value);

    static void SetAttr(py::object self, const std::string& s, pybind11::object value);
    static py::object GetAttr(Base* self, const std::string& s, bool doThrowException=true);
    static void SetAttrFromArray(py::object self, const std::string& s, const pybind11::array &value);

    /// Set the data field value from the array.
    static void SetDataFromArray(BaseData* data, const py::array& value);
    static bool SetData(BaseData* data, pybind11::object value);

    static py::list getDataFields(Base& self);
    static py::list getLinks(Base& self);
    static void addData(py::object py_self, const std::string& name, py::object value = py::object(), py::object defaultValue = py::object(), const std::string& help = "", const std::string& group = "Property", std::string type = "");
    static void addDataFromData(Base* self, py::object d);
    static py::list __dir__(Base* self);
    static py::object __getattr__(py::object self, const std::string& s);
    static void __setattr__(py::object self, const std::string& s, py::object value);
    static py::object getData(Base& self, const std::string&);
};


void moduleAddBase(py::module& m);

bool isProtectedKeyword(const std::string& name);

} /// namespace sofapython3
