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

namespace sofapython3 {

/**
 * This is the main pybind11 type holder that will manage the underlying reference counter of the SOFA C++ objects. It
 * should be used when creating bindings for a C++ class that inherits sofa::core::objectmodel::Base.
 *
 * \note There is a problem in pybind11 which sometime does a kind of slicing between the python object and its bound
 *       C++ object. This usually happens when instantiating a virtual (trampoline) class in python that then calls the
 *       new operator in the pybind11::init() and return the newly created object. Python will sometime destroy the
 *       initial object before the C++ one is created. We go around this by keeping here a reference to the initial
 *       python object, hence incrementing its reference counter by one.
 *
 *       See https://github.com/pybind/pybind11/issues/1546 for more details.
 * @tparam T
 */

template <typename T>
struct py_shared_ptr : public sofa::core::sptr<T> {
    using Base = sofa::core::sptr<T>;
    using Base::Base;

    py_shared_ptr(const sofa::core::sptr<T> & sptr) : Base(sptr) {}

    py_shared_ptr(const sofa::core::sptr<T> & sptr, T * ptr) : Base(ptr) {}

    py_shared_ptr(const py_shared_ptr & py_ptr) : Base(py_ptr) {
        p_object = py_ptr.p_object;
    }

    py_shared_ptr(py_shared_ptr & py_ptr) : Base(py_ptr) {
        p_object = py_ptr.p_object;
    }

    void set_object(const pybind11::object & o) {
        p_object = o;
    }

private:
    pybind11::object p_object;
};

} // namespace sofapython3

// Type caster for SP3 and SOFA holders
namespace pybind11::detail {
/**
 * This is an alias type caster that converts sofa::core::sptr<T> to sofapython3::py_shared_ptr<T>.
 * It is needed for when bound C++ function returns sofa::core::sptr instead of py_shared_ptr while the holder
 * type was set to the latter.
 * @tparam T
 */
template <typename T>
class type_caster<sofa::core::sptr<T>> {

PYBIND11_TYPE_CASTER (sofa::core::sptr<T> , _("sofa::core::sptr<T>"));

    using BaseCaster = copyable_holder_caster<T, sofapython3::py_shared_ptr<T>>;

    bool load (pybind11::handle src, bool b)
    {
        // First make sure the py::object is an instanced of sofapython3::py_shared_ptr<T>
        BaseCaster bc;
        bool success = bc.load (src, b);
        if (!success) {
            return false;
        }

        // Convert the holder_caster to a sofapython3::py_shared_ptr<T> instance
        auto base_ptr = static_cast<sofapython3::py_shared_ptr<T>> (bc);

        // Take ownership of the py::object
        auto h = BaseCaster::cast(base_ptr, return_value_policy(), handle());
        auto py_obj = reinterpret_borrow<object>(h);

        // Save a copy into the holder so that it doesn't get deleted and thereby creating slicing
        base_ptr.set_object(py_obj);

        // Set the value to the ptr
        value = sofa::core::sptr<T> (base_ptr);
        return true;
    }

    static handle cast (sofa::core::sptr<T> sp,
                        return_value_policy rvp,
                        handle h)
    {
        return BaseCaster::cast (sp, rvp, h);
    }
};

template <typename T>
struct always_construct_holder<sofapython3::py_shared_ptr<T>> : always_construct_holder<void, true>  { };

/**
 * Main type caster that converts python objects to C++ sofapython3::py_shared_ptr<T> objects, and the inverse.
 * To avoid slicing (see sofapython3::py_shared_ptr), it will preserve the initial python object when the initial
 * python -> C++ conversion happens.
 * @tparam T
 */
template <typename T>
class type_caster<sofapython3::py_shared_ptr<T>> {

PYBIND11_TYPE_CASTER (sofapython3::py_shared_ptr<T> , _("sofapython3::py_shared_ptr<T>"));

    using BaseCaster = copyable_holder_caster<T, sofapython3::py_shared_ptr<T>>;

    bool load (pybind11::handle src, bool b)
    {
        // First make sure the py::object is an instanced of sofapython3::py_shared_ptr<T>
        BaseCaster bc;
        bool success = bc.load (src, b);
        if (!success) {
            return false;
        }

        // Convert the holder_caster to a sofapython3::py_shared_ptr<T> instance
        auto base_ptr = static_cast<sofapython3::py_shared_ptr<T>> (bc);

        // Take ownership of the py::object
        auto h = BaseCaster::cast(base_ptr, return_value_policy(), handle());
        auto py_obj = reinterpret_borrow<object>(h);

        // Save a copy into the holder so that it doesn't get deleted and thereby creating slicing
        base_ptr.set_object(py_obj);

        // Set the value to the ptr
        value = base_ptr;
        return true;
    }

    static handle cast (sofapython3::py_shared_ptr<T> sp,
                        return_value_policy rvp,
                        handle h)
    {
        return BaseCaster::cast (sp, rvp, h);
    }
};

template <typename T>
struct is_holder_type<T, sofapython3::py_shared_ptr<T>> : std::true_type {};
}

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
};


void moduleAddBase(pybind11::module& m);

} /// namespace sofapython3
