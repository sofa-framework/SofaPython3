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
    static BaseData* addData(py::object py_self, const std::string& name, py::object value = py::object(), py::object defaultValue = py::object(), const std::string& help = "", const std::string& group = "Property", std::string type = "");
    static BaseData* addDataFromData(Base* self, py::object d);
    static py::list __dir__(Base* self);
    static py::object __getattr__(py::object self, const std::string& s);
    static void __setattr__(py::object self, const std::string& s, py::object value);
    static py::object getData(Base& self, const std::string&);

    static py::object getLoggedMessagesAsString(Base& self);
    static py::object countLoggedMessages(Base& self);
    static py::object clearLoggedMessages(Base& self);
};


void moduleAddBase(py::module& m);

} /// namespace sofapython3
