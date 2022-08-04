/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2021 CNRS, University of Lille, INRIA                  *
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


#include <sofa/defaulttype/DataTypeInfo.h>
using sofa::defaulttype::AbstractTypeInfo;

#include <sofa/core/objectmodel/BaseData.h>
using sofa::core::objectmodel::BaseData;

#include <sofa/core/objectmodel/BaseObject.h>
using  sofa::core::objectmodel::BaseObject;

#include <sofa/core/objectmodel/BaseNode.h>
using  sofa::core::objectmodel::BaseNode;

#include <SofaPython3/DataHelper.h>
#include <SofaPython3/PythonFactory.h>
#include "../Binding_Base.h"
#include "../Binding_BaseData.h"
#include "Binding_DataContainer.h"
#include "Binding_DataContainer_doc.h"

#include <sofa/type/BoundingBox.h>

namespace sofapython3
{

void moduleAddDataContainer(py::module& m)
{
    py::class_<DataContainer, BaseData, std::unique_ptr<DataContainer, py::nodelete>> p(m, "DataContainer",
                                                                  py::buffer_protocol(), sofapython3::doc::datacontainer::Class);

    PythonFactory::registerType("DataContainer", [](BaseData* data) -> py::object {
        return py::cast(reinterpret_cast<DataContainer*>(data));
    });

    p.def("__getitem__", [](DataContainer* self, py::object& index) -> py::object
    {
        py::array a = getPythonArrayFor(self);
        py::buffer_info parentinfo = a.request();

        return py::reinterpret_steal<py::object>(PyObject_GetItem(a.ptr(), index.ptr() ));
    });

    p.def("__setitem__", [](DataContainer* self, size_t& index, py::object& value)
    {
        scoped_writeonly_access access(self);
        setItem(getPythonArrayFor(self), py::slice(index, index+1, 1), value);
        return py::none();
    });

    p.def("__setitem__", [](DataContainer* self, py::slice& slice, py::object& value) -> py::object
    {
        scoped_writeonly_access access(self);

        setItem(getPythonArrayFor(self), slice, value);
        return py::none();
    });

    p.def("__setitem__", [](DataContainer* self, py::tuple key, py::object& value)
    {
        scoped_writeonly_access access(self);

        py::array a=getPythonArrayFor(self);
        py::slice s0=toSlice(key[0]);
        py::slice s1=toSlice(key[1]);

        setItem2D(a, s0, s1, value);

        return py::none();
    });

    p.def("apply", [](DataContainer* self, py::function f)
    {
        scoped_write_access access(self);
        py::array a = getPythonArrayFor(self);

        auto aa=a.mutable_unchecked<double>();
        for(auto i=0;i<aa.shape(0);++i){
            for(auto j=0;j<aa.shape(1);++j){
                aa(i,j) = py::cast<double>(f(i,j, aa(i,j)));
            }
        }
    });

    p.def("__str__", [](BaseData* self)
    {
        std::stringstream tmp;
        tmp << "Sofa.Core.DataContainer <name='" << self->getName() << "', address=" << self << ">";
        return py::str(tmp.str());
    });

    p.def("__repr__", [](BaseData* self)
    {
        return py::repr(convertToPython(self));
    });

    p.def("toList", [](DataContainer* self){
        return convertToPython(self);
    });

    /// This is the standard terminology in numpy so we keep it for consistency purpose.
    p.def_property_readonly("ndim", &getNDim, sofapython3::doc::datacontainer::ndim);    /// 1 or 2D array
    p.def_property_readonly("shape", [](BaseData* self)
    {
        std::tuple<int, int> shape = getShape(self);
        if(std::get<1>(shape)==-1)
        {
            py::tuple r {1};
            r[0] = std::get<0>(shape);
            return r;
        }
        py::tuple r {2};
        r[0] = std::get<0>(shape);
        r[1] = std::get<1>(shape);
        return r;
    }, sofapython3::doc::datacontainer::shape);  /// array containing the size in each dimmension

    p.def_property_readonly("size", &getSize, sofapython3::doc::datacontainer::size);    /// get the total number of elements
    p.def("__len__", [](DataContainer* self)                     /// In numpy the len is the number of element in the first
                                                                 /// dimmension.
    {
        auto nfo = self->getValueTypeInfo();
        auto itemNfo = nfo->BaseType();

        /// This is a 1D container
        if( itemNfo->Container() )
            return nfo->size(self->getValueVoidPtr()) / itemNfo->size();

        return nfo->size(self->getValueVoidPtr());
    }, sofapython3::doc::datacontainer::__len__);

    p.def("array", [](DataContainer* self){
        // in array construction, array increases the reference counter of the capsule and sets it to the owner of the buffer
        auto capsule = py::capsule(self->getOwner());
        py::buffer_info ninfo = toBufferInfo(*self);
        py::array a(pybind11::dtype(ninfo), ninfo.shape,
                    ninfo.strides, ninfo.ptr, capsule);
        a.attr("flags").attr("writeable") = false;
        return a;
    });

    p.def("writeable", [](DataContainer* self, py::object f) -> DataContainerContext*
    {
        if(self!=nullptr){
            return new DataContainerContext(self, f);
        }
        // pybind11 automatically converts this nullptr into a python None object
        return nullptr;
    });

    p.def("writeable", [](DataContainer* self) -> DataContainerContext*
    {
        if(self!=nullptr){
            return new DataContainerContext(self, py::none());
        }
        // pybind11 automatically converts this nullptr into a python None object
        return nullptr;
    });

    p.def("__iadd__", [](DataContainer* self, py::object value)
    {
        /// Acquire an access to the underlying data. As this is a read+write access we
        /// use the scoped_write_access object.
        scoped_write_access access(self);

        /// Search if this container object has a PythonArray in the case, if this is not the case
        /// create one.
        py::array p = getPythonArrayFor(self);

        if(py::isinstance<DataContainer>(value))
            value = getPythonArrayFor(py::cast<BaseData*>(value));

        /// Returns a new reference on the result.
        /// We don't want to keep this reference so we decref it to avoid memory leak.
        Py_DECREF(PyNumber_InPlaceAdd(p.ptr(), value.ptr()));

        /// Instead, returns the self object as we are in an in-place add operator.
        return self;
    });

    p.def("__add__", [](DataContainer* self, py::object value)
    {
        /// Acquire an access to the underlying data. As this is a read only access we
        /// use the scoped_read_access object. This imply that the data will updates the content
        /// of this object.
        scoped_read_access access(self);
        py::array p = getPythonArrayFor(self);

        if(py::isinstance<DataContainer>(value))
            value = getPythonArrayFor(py::cast<BaseData*>(value));

        return py::reinterpret_steal<py::object>(PyNumber_Add(p.ptr(), value.ptr()));
    });

    p.def("__isub__", [](DataContainer* self, py::object value)
    {
        /// Acquire an access to the underlying data. As this is a read+write access we
        /// use the scoped_write_access object.
        scoped_write_access access(self);

        /// Search if this container object has a PythonArray in the case, if this is not the case
        /// create one.
        py::array p = getPythonArrayFor(self);

        if(py::isinstance<DataContainer>(value))
            value = getPythonArrayFor(py::cast<BaseData*>(value));

        /// Returns a new reference on the result.
        /// We don't want to keep this reference so we decref it to avoid memory leak.
        Py_DECREF(PyNumber_InPlaceSubtract(p.ptr(), value.ptr()));

        /// Instead, returns the self object as we are in an in-place add operator.
        return self;
    });

    p.def("__sub__", [](DataContainer* self, py::object value)
    {
        /// Acquire an access to the underlying data. As this is a read only access we
        /// use the scoped_read_access object. This imply that the data will updates the content
        /// of this object.
        scoped_read_access access(self);
        py::array p = getPythonArrayFor(self);

        if(py::isinstance<DataContainer>(value))
            value = getPythonArrayFor(py::cast<BaseData*>(value));

        return py::reinterpret_steal<py::object>(PyNumber_Subtract(p.ptr(), value.ptr()));
    });

    p.def("__imul__", [](DataContainer* self, py::object value)
    {
        /// Acquire an access to the underlying data. As this is a read+write access we
        /// use the scoped_write_access object.
        scoped_write_access access(self);

        /// Search if this container object has a PythonArray in the case, if this is not the case
        /// create one.
        if( !hasArrayFor(self) )
            throw py::type_error("NOOO");

        py::array p = getPythonArrayFor(self);

        if(py::isinstance<DataContainer>(value))
        {
            value = getPythonArrayFor(py::cast<BaseData*>(value));
        }

        /// Returns a new reference on the result.
        /// We don't want to keep this reference so we decref it to avoid memory leak.
        Py_DECREF(PyNumber_InPlaceMultiply(p.ptr(), value.ptr()));

        /// Instead, returns the self object as we are in an in-place operator.
        return py::cast(self);
    });

    p.def("__mul__", [](DataContainer* self, py::object value)
    {
        /// Acquire an access to the underlying data. As this is a read only access we
        /// use the scoped_read_access object. This imply that the data will updates the content
        /// of this object.
        scoped_read_access access(self);
        py::array p = getPythonArrayFor(self);

        if(py::isinstance<DataContainer>(value))
            value = getPythonArrayFor(py::cast<BaseData*>(value));

        return py::reinterpret_steal<py::object>(PyNumber_Multiply(p.ptr(), value.ptr()));
    });
}

void moduleAddDataContainerContext(py::module& m)
{
    py::class_<DataContainerContext> wa(m, "DataContainerContextManager");
    wa.def("__enter__", [](DataContainerContext& wa)
    {
        wa.data->beginEditVoidPtr();
        py::array mainbuffer = getPythonArrayFor(wa.data);
        py::buffer_info info = mainbuffer.request();
        wa.wrap = py::array(pybind11::dtype(info), info.shape,
                            info.strides, info.ptr, mainbuffer);

        if(!wa.fct.is_none())
            wa.wrap = wa.fct(wa.wrap);

        return wa.wrap;
    });

    wa.def("__exit__",
           [](DataContainerContext& wa, py::object type, py::object value, py::object traceback)
    {
        SOFA_UNUSED(type);
        SOFA_UNUSED(value);
        SOFA_UNUSED(traceback);
        wa.wrap.attr("flags").attr("writeable") = false;
        wa.data->endEditVoidPtr();
    });
}

}
