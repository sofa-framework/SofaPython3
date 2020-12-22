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

#include <pybind11/pybind11.h>

#include <pybind11/numpy.h>

#include <sofa/core/objectmodel/BaseData.h>
using sofa::core::objectmodel::BaseData;

#include <sofa/core/objectmodel/BaseLink.h>
using sofa::core::objectmodel::BaseLink;

#include <sofa/helper/accessor.h>
using sofa::helper::WriteOnlyAccessor;

#include <SofaPython3/PythonFactory.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_Base_doc.h>
#include <SofaPython3/Sofa/Core/Binding_DataDict.h>
#include "Data/Binding_DataContainer.h"

#include <sofa/simulation/Node.h>
using sofa::simulation::Node;

#include <SofaPython3/DataHelper.h>

/// Bind the python's attribute error
namespace pybind11 { PYBIND11_RUNTIME_EXCEPTION(attribute_error, PyExc_AttributeError) }

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

/// Bring pybind11 literals
using namespace pybind11::literals;

namespace sofapython3
{

///@brief return a py::object for the provided name 's'.
///
/// It is possible to control if the function is throwing an exception when
/// the provided name is not found in the 'self' object.
py::object BindingBase::GetAttr(Base* self, const std::string& s, bool doThrowException)
{
    /// I'm not sure implicit behavior is nice but we could do:
    ///    - The attribute is a data,
    ///         returns it if it is a container
    ///         returns the value/specific binding otherwise
    ///    - The attribute is a link, return it.
    ///    - The attribute is an object or a child return it.
    ///    - The attribute is not existing:
    ///                raise an exception or search using difflib for close match.
    if(self==nullptr)
        throw py::attribute_error("Cannot get a Sofa attribute from None.");

    /// We are selecting first into data, then into link. Because
    /// this seems to be the most common use-case.

    /// Search if there is a data with the given name.
    /// If this is the case returns the corresponding python type.
    if(BaseData* d = self->findData(s)){
        return PythonFactory::toPython(d);
    }

    /// Search if there is a link with the given name.
    /// If this is the case returns the corresponding python type.
    if(BaseLink* l = self->findLink(s))
        return py::cast(l);

    /// Search if we are quering for a 'magic' and private __data__ property
    /// this one allows to traverse all the data in the object
    if( s == "__data__")
        return py::cast( DataDict(self) );

    if(doThrowException)
        throw py::attribute_error("Missing attribute: "+s);

    return py::none();
}

bool BindingBase::SetData(BaseData* d, py::object value)
{
    if(d==nullptr)
        return false;

    const AbstractTypeInfo& nfo{ *(d->getValueTypeInfo()) };

    PythonFactory::fromPython(d, value);
    return true;
}


void BindingBase::SetAttr(py::object self, const std::string& s, py::object value)
{
    Base* self_d = py::cast<Base*>(self);
    BaseData* d = self_d->findData(s);

    if(d!=nullptr)
    {
        SetData(d, value);
        return;
    }

    BaseLink* l = self_d->findLink(s);
    if(l!=nullptr)
    {
        return;
    }

    /// We are falling back to dynamically adding the objet into the object dict.
    py::dict t = self.attr("__dict__");
    if(!t.is_none())
    {
        t[s.c_str()] = value;
        return;
    }

    /// Well this should never happen unless there is no __dict__
    throw py::attribute_error("Unable to set attribute '"+s+"', unknow data type");
}

void BindingBase::SetAttr(Base& self, const std::string& s, py::object value)
{
    BaseData* d = self.findData(s);

    if(d!=nullptr)
    {
        const AbstractTypeInfo& nfo{ *(d->getValueTypeInfo()) };

        /// We go for the container path.
        if(nfo.Container())
        {
            PythonFactory::fromPython(d,value);
            return;
        }
        PythonFactory::fromPython(d, value);
        return;
    }

    BaseLink* l = self.findLink(s);
    if(l!=nullptr)
    {
        return;
    }

    throw py::attribute_error(self.name.getValue() + " has no data field nor link named '" + s + "'");
}

void BindingBase::SetDataFromArray(BaseData* data, const py::array& value)
{
    if(data==nullptr)
        return;

    const AbstractTypeInfo& nfo{ *(data->getValueTypeInfo()) };

    /// We go for the container path.
    if(nfo.Container())
    {
        py::array dst = getPythonArrayFor(data);
        py::buffer_info dstinfo = dst.request();

        py::array src = value;
        py::buffer_info srcinfo = src.request();
        if( srcinfo.ptr == dstinfo.ptr )
        {
            /// Increment the change counter so other data field can keep track of
            /// what happens.
            data->beginEditVoidPtr();
            data->endEditVoidPtr();
            return;
        }

        /// Invalid dimmensions
        if( srcinfo.ndim != dst.ndim() )
            throw py::type_error("Invalid dimension");

        bool needResize = false;
        size_t resizeShape=0;
        size_t srcSize = 1;
        for(auto i=0;i<srcinfo.ndim;++i){
            srcSize *= srcinfo.shape[i];
            if( srcinfo.shape[i] != dstinfo.shape[i])
            {
                resizeShape = i;
                needResize = true;
            }
        }

        if(needResize)
        {
            if(nfo.FixedSize())
                throw py::index_error("The destination is not large enough and cannot be resized. Please clamp the source data set before setting.");

            if(resizeShape != 0)
                throw py::index_error("The destination can only be resized on the first dimension. ");

            /// Change the allocated memory of the data field, then update the
            /// cache entry so keep up with the changes. As we use dstinfo in the following
            /// we also update it.
            nfo.setSize(data->beginEditVoidPtr(), srcSize);
            dst = resetArrayFor(data);
            dstinfo=dst.request();
        }

        bool sameDataType = (srcinfo.format == dstinfo.format);
        if(sameDataType && (nfo.BaseType()->FixedSize() || nfo.SimpleCopy()))
        {
            scoped_writeonly_access guard(data);
            memcpy(dstinfo.ptr, srcinfo.ptr, srcSize*dstinfo.itemsize);
            return;
        }

        /// In this case we go for the fast path.
        if(nfo.SimpleLayout())
        {
            if(srcinfo.format=="d")
                return copyScalar<double>(data, nfo, src);
            else if(srcinfo.format=="f")
                return copyScalar<float>(data, nfo, src);
            else
                std::cout << "SetAttrFromArray :: unsupported fileformat" << std::endl ;
        }

    }

    PythonFactory::fromPython(data, value);
    return;
}

void BindingBase::SetAttrFromArray(py::object self, const std::string& s, const py::array& value)
{
    /// I'm not sure implicit behavior is nice but we could do:
    ///    - The attribute is a data, set its value.
    ///          If the data is a container...check dimmensions and do type coercion.
    ///    - The attribute is a link, set its value.
    ///    - The attribute is an object or a child, raise an exception.
    ///    - The attribute is not existing, add it has data with type deduced from value ?
    Base& self_base = py::cast<Base&>(self);
    BaseData* data = self_base.findData(s);

    /// If there is a data with the given name we set the data value.
    if(data!=nullptr)
    {
        SetDataFromArray(data, value);
        return;
    }

    /// We are falling back to dynamically adding the objet into the object dict.
    py::dict t = self.attr("__dict__");
    if(!t.is_none())
    {
        t[s.c_str()] = value;
        return;
    }

    /// Well this should never happen unless there is no __dict__
    throw py::attribute_error("");
}

py::list BindingBase::getDataFields(Base& self)
{
    py::list list;
    for(auto i : self.getDataFields())
    {
        list.append(PythonFactory::toPython(i));
    }
    return list;
}

py::list BindingBase::getLinks(Base& self)
{
    py::list list;
    for(auto i : self.getLinks())
    {
        list.append(i);
    }
    return list;
}

BaseData* BindingBase::addData(py::object py_self, const std::string& name, py::object value, py::object defaultValue, const std::string& help, const std::string& group, std::string type)
{
    return sofapython3::addData(py_self, name, value, defaultValue, help, group, type);
}

BaseLink* BindingBase::addLink(py::object py_self, const std::string& name, py::object value, const std::string& help)
{
    return sofapython3::addLink(py_self, name, value, help);
}

BaseData* BindingBase::addDataFromData(Base* self, py::object d)
{
    BaseData* data = py::cast<BaseData*>(d);
    if (!data)
        throw py::type_error("Argument is not a Data!");

    if (data->getOwner() == nullptr){
        self->addData(data, data->getName());
        return data;
    }

    BaseData* newData = data->getNewInstance();
    newData->setOwner(self);
    newData->setParent(data);
    newData->setName(data->getName());
    return newData;
}

py::list BindingBase::__dir__(Base* self)
{
    py::list list;
    for(auto i : self->getDataFields())
    {
        list.append(i->getName());
    }
    return list;
}

py::object BindingBase::__getattr__(py::object self, const std::string& s)
{
    py::object res = BindingBase::GetAttr( py::cast<Base*>(self), s, false );
    if( res.is_none() )
    {
        return self.attr("__dict__")[s.c_str()];
    }

    return res;
}

void BindingBase::__setattr__(py::object self, const std::string& s, py::object value)
{
    if(py::isinstance<DataContainer>(value))
    {
        BaseData* data = py::cast<BaseData*>(value);
        py::array a = getPythonArrayFor(data);
        BindingBase::SetAttrFromArray(self,s, a);
        return;
    }

    if(py::isinstance<py::array>(value))
    {
        BindingBase::SetAttrFromArray(self,s, py::cast<py::array>(value));
        return;
    }

    BindingBase::SetAttr(self,s,value);
}

py::object BindingBase::getLoggedMessagesAsString(Base& self)
{
    return py::str(self.getLoggedMessagesAsString());
}

py::object BindingBase::countLoggedMessages(Base& self)
{
    return py::int_(self.countLoggedMessages());
}

py::object BindingBase::clearLoggedMessages(Base& self)
{
    self.clearLoggedMessages();
    return py::none();
}

py::object BindingBase::getData(Base& self, const std::string& s)
{
    BaseData* d = self.findData(s);
    if(d!=nullptr)
    {
        return py::cast(d);
    }
    return py::none();
}


std::string BindingBase::getPathName(Base& self)
{
    return self.toBaseNode() ? self.toBaseNode()->getPathName() : self.toBaseObject()->getPathName();
}
    
py::object BindingBase::setDataValues(Base& self, py::kwargs kwargs)
{
    for(auto key : kwargs)
    {
        BaseData* d = self.findData(py::cast<std::string>(key.first));
        if(d!=nullptr)
            PythonFactory::fromPython(d, py::cast<py::object>(key.second));
        else
            throw py::attribute_error("There is no data field named: "+py::cast<std::string>(key.first));
    }
    return py::none();
}

void moduleAddBase(py::module &m)
{
    py::class_<Base, py_shared_ptr<Base>> base(m, "Base", py::dynamic_attr(), doc::base::BaseClass);
    /// set & get the name as string. The alternative is to access the data field using
    /// obj.name.value = "aName"
    base.def("getName", [](Base& b){ return b.getName(); }, sofapython3::doc::base::getName);
    base.def("setName", [](Base& b, const std::string& s){ b.setName(s); }, sofapython3::doc::base::setName);
    base.def("getClassName",&Base::getClassName, sofapython3::doc::base::getClassName);
    base.def("getTemplateName",&Base::getTemplateName, sofapython3::doc::base::getTemplateName);
    base.def("getClass", &Base::getClass, pybind11::return_value_policy::reference, sofapython3::doc::base::getClass);
    base.def("getDefinitionSourceFilePos", &Base::getDefinitionSourceFilePos, sofapython3::doc::base::getDefinitionSourceFilePos);
    base.def("getDefinitionSourceFileName", &Base::getDefinitionSourceFileName, sofapython3::doc::base::getDefinitionSourceFileName);
    base.def("getInstanciationSourceFilePos", &Base::getInstanciationSourceFilePos, sofapython3::doc::base::getInstanciationSourceFilePos);
    base.def("getInstanciationFileName", &Base::getInstanciationSourceFileName, sofapython3::doc::base::getInstanciationSourceFilePos);

    base.def("setDefinitionSourceFilePos", &Base::setDefinitionSourceFilePos, sofapython3::doc::base::setDefinitionSourceFilePos);
    base.def("setDefinitionSourceFileName", &Base::setDefinitionSourceFileName, sofapython3::doc::base::setDefinitionSourceFileName);
    base.def("setInstanciationSourceFilePos", &Base::setInstanciationSourceFilePos, sofapython3::doc::base::setInstanciationSourceFilePos);
    base.def("setInstanciationSourceFileName", &Base::setInstanciationSourceFileName, sofapython3::doc::base::setInstanciationSourceFilePos);

    base.def("getDataFields", &BindingBase::getDataFields, pybind11::return_value_policy::reference, sofapython3::doc::base::getDataFields);
    base.def("findLink", &Base::findLink, pybind11::return_value_policy::reference, sofapython3::doc::base::findLink);
    base.def("getLinks", &BindingBase::getLinks, pybind11::return_value_policy::reference, sofapython3::doc::base::getLinks);
    base.def("addData", &BindingBase::addData, "name"_a, "value"_a = py::none(), "default"_a = py::none(), "help"_a = "", "group"_a = "", "type"_a = "", sofapython3::doc::base::addData);
    base.def("addData", &BindingBase::addDataFromData, sofapython3::doc::base::addDataInitialized);
    base.def("addLink", &BindingBase::addLink, "name"_a, "value"_a = py::none(), "help"_a = "", sofapython3::doc::base::addLink);
    base.def("__getattr__", &BindingBase::__getattr__);
    base.def("__setattr__", &BindingBase::__setattr__);
    base.def("getData", &BindingBase::getData, sofapython3::doc::base::getData);
    base.def("findData", &Base::findData, pybind11::return_value_policy::reference, sofapython3::doc::base::findData);
    base.def("__dir__", &BindingBase::__dir__);
    base.def("getLoggedMessagesAsString", &BindingBase::getLoggedMessagesAsString, sofapython3::doc::base::getLoggedMessagesAsString);
    base.def("countLoggedMessages", &BindingBase::countLoggedMessages, sofapython3::doc::base::countLoggedMessages);
    base.def("clearLoggedMessages", &BindingBase::clearLoggedMessages, sofapython3::doc::base::clearLoggedMessages);
    base.def("getPathName", &BindingBase::getPathName);
    base.def("setDataValues", &BindingBase::setDataValues, sofapython3::doc::base::setDataValues);
}

} /// namespace sofapython3
