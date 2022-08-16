/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2021 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseData.h>

namespace sofapython3 {

template <typename T>
using py_shared_ptr = sofa::core::sptr<T>;

} // namespace sofapython3

PYBIND11_DECLARE_HOLDER_TYPE(T, sofapython3::py_shared_ptr<T>, true)

namespace sofapython3 {

class BindingBase
{
public:
    static void SetAttr(sofa::core::objectmodel::Base& self, const std::string& s, pybind11::object value);

    static void SetAttr(pybind11::object self, const std::string& s, pybind11::object value);
    static pybind11::object GetAttr(sofa::core::objectmodel::Base* self, const std::string& s, bool doThrowException=true);
    static void SetAttrFromArray(pybind11::object self, const std::string& s, const pybind11::array &value);

    /// Set the data field value from the array.
    static void SetDataFromArray(sofa::core::objectmodel::BaseData* data, const pybind11::array& value);
    static bool SetData(sofa::core::objectmodel::BaseData* data, pybind11::object value);
    static bool SetLink(sofa::core::objectmodel::BaseLink* link, pybind11::object value);
    static pybind11::object setDataValues(sofa::core::objectmodel::Base& self, pybind11::kwargs kwargs);

    static pybind11::list getDataFields(sofa::core::objectmodel::Base& self);
    static pybind11::list getLinks(sofa::core::objectmodel::Base& self);
    static sofa::core::objectmodel::BaseData* addData(pybind11::object py_self, const std::string& name, pybind11::object value = pybind11::object(), pybind11::object defaultValue = pybind11::object(), const std::string& help = "", const std::string& group = "Property", std::string type = "");
    static sofa::core::objectmodel::BaseData* addDataFromData(sofa::core::objectmodel::Base* self, pybind11::object d);
    static sofa::core::objectmodel::BaseLink* addLink(pybind11::object py_self, const std::string& name, pybind11::object value, const std::string& help);
    static pybind11::list __dir__(sofa::core::objectmodel::Base* self);
    static pybind11::object __getattr__(pybind11::object self, const std::string& s);
    static void __setattr__(pybind11::object self, const std::string& s, pybind11::object value);
    static pybind11::object getData(sofa::core::objectmodel::Base& self, const std::string&);

    static pybind11::object getLoggedMessagesAsString(sofa::core::objectmodel::Base& self);
    static pybind11::object countLoggedMessages(sofa::core::objectmodel::Base& self);
    static pybind11::object clearLoggedMessages(sofa::core::objectmodel::Base& self);
    static std::string getPathName(sofa::core::objectmodel::Base& self);
    static std::string getLinkPath(sofa::core::objectmodel::Base& self);
};


void moduleAddBase(pybind11::module& m);

} /// namespace sofapython3
