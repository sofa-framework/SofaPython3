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

#include <pybind11/numpy.h>
#include <pybind11/eval.h>

#include <sofa/defaulttype/DataTypeInfo.h>

#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/core/objectmodel/BaseObject.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_BaseData.h>
#include <SofaPython3/Sofa/Core/Binding_BaseData_doc.h>
#include <SofaPython3/Sofa/Core/Data/Binding_DataContainer.h>
#include <SofaPython3/DataHelper.h>
#include <SofaPython3/PythonFactory.h>

/// Bind the python's attribute error
namespace pybind11 { PYBIND11_RUNTIME_EXCEPTION(attribute_error, PyExc_AttributeError) }
/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

using namespace sofa::core::objectmodel;
using sofa::defaulttype::AbstractTypeInfo;

namespace sofapython3
{

std::string getPathName(BaseData& self)
{
    Base* b= self.getOwner();
    std::string prefix = getPathTo(b);
    return prefix+"."+self.getName();
}

std::string getLinkPath(BaseData& self)
{
    return "@"+getPathName(self);
}

bool hasChanged(BaseData& data)
{
    if (data.isDirty()) {
        data.update();
        return true;
    }
    return false;
}

bool isSet(BaseData& data){ return data.isSet(); }

py::str __str__(BaseData* self)
{
    std::stringstream tmp;
    tmp << "Sofa.Core.Data <'" << self->getName() << "', " << self << ">";
    return py::str(tmp.str());
}

py::str __repr__(BaseData* self)
{
    return py::repr(PythonFactory::toPython(self));
}

py::list toList(BaseData* self)
{
    return convertToPython(self);
}

py::array array(BaseData* self)
{
    auto capsule = py::capsule(new Base::SPtr(self->getOwner()));
    py::buffer_info ninfo = toBufferInfo(*self);
    py::array a(pybind11::dtype(ninfo), ninfo.shape,
                ninfo.strides, ninfo.ptr, capsule);
    a.attr("flags").attr("writeable") = false;
    return a;
}

py::object writeableArrayWithType(BaseData* self, py::object f)
{
    if(self!=nullptr)
        return py::cast(new DataContainerContext(self, f));

    return py::none();
}

py::object writeableArray(BaseData* self)
{
    if(self!=nullptr)
        return py::cast(new DataContainerContext(self, py::none()));

    return py::none();
}

void __setattr__(py::object self, const std::string& s, py::object value)
{
    BaseData* selfdata = py::cast<BaseData*>(self);

    if(py::isinstance<DataContainer>(value))
    {
        py::array a = getPythonArrayFor(py::cast<DataContainer*>(value));
        BindingBase::SetDataFromArray(selfdata, a);
        return;
    }

    if(py::isinstance<py::array>(value))
    {
        BindingBase::SetDataFromArray(selfdata, py::cast<py::array>(value));
        return;
    }
    BindingBase::SetAttr(py::cast(selfdata->getOwner()),selfdata->getName(),value);
}

py::object __getattr__(py::object self, const std::string& s)
{
    /// If this is data.value we returns the content value of the data field converted into
    /// a python object that is easy to manipulate. The conversion is done with the toPython
    /// function.
    if(s == "value")
        return PythonFactory::valueToPython_ro(py::cast<BaseData*>(self));

    /// BaseData does not support dynamic attributes, if you think this is an important feature
    /// please request for its integration.
    throw py::attribute_error("There is no attribute '"+s+"'");
}

void setParent(BaseData* self, BaseData* parent)
{
    self->setParent(parent);
}

bool hasParent(BaseData *self)
{
    return (self->getParent() != nullptr);
}

void updateIfDirty(BaseData* self)
{
    return self->updateIfDirty();
}

bool isDirty(BaseData* self)
{
    return self->isDirty();
}

py::object getOwner(BaseData& self)
{
    return PythonFactory::toPython(self.getOwner());
}

void moduleAddBaseData(py::module& m)
{
    /// Register the BaseData binding into the pybind11 system.
    py::class_<BaseData, std::unique_ptr<sofa::core::objectmodel::BaseData, pybind11::nodelete>> data(m, "Data", sofapython3::doc::baseData::BaseDataClass);
    data.def("getName", [](BaseData& b){ return b.getName(); }, sofapython3::doc::baseData::getName);
    data.def("setName", [](BaseData& b, const std::string& s){ b.setName(s); }, sofapython3::doc::baseData::setName);
    data.def("getCounter", [](BaseData& self) { return self.getCounter(); }, sofapython3::doc::baseData::getCounter);
    data.def("getHelp", &BaseData::getHelp, sofapython3::doc::baseData::getHelp);
    data.def("unset", [](BaseData& b){ b.unset(); }, sofapython3::doc::baseData::unset);
    data.def("getOwner", &getOwner, sofapython3::doc::baseData::getOwner);
    data.def("getParent", &BaseData::getParent, sofapython3::doc::baseData::getParent);
    data.def("typeName", [](BaseData& data){ return data.getValueTypeInfo()->name(); }, sofapython3::doc::baseData::typeName);
    data.def("getPathName", getPathName, sofapython3::doc::baseData::getPathName);
    data.def("getLinkPath", getLinkPath, sofapython3::doc::baseData::getLinkPath);
    data.def("hasChanged", hasChanged, sofapython3::doc::baseData::hasChanged);
    data.def("isSet", isSet, sofapython3::doc::baseData::isSet);
    data.def("toList", toList, sofapython3::doc::baseData::toList);
    data.def("array", array, sofapython3::doc::baseData::array);
    data.def("writeableArray", writeableArrayWithType, sofapython3::doc::baseData::writeableArrayArg);
    data.def("writeableArray", writeableArray, sofapython3::doc::baseData::writeableArray);
    data.def("__str__", __str__);
    data.def("__repr__", __repr__);
    data.def("__setattr__", __setattr__);
    data.def("__getattr__", __getattr__);
    data.def("getValueString",&BaseData::getValueString, sofapython3::doc::baseData::getValueString);
    data.def("getValueTypeString", &BaseData::getValueTypeString, sofapython3::doc::baseData::getValueTypeString);
    data.def("isPersistent", &BaseData::isPersistent, sofapython3::doc::baseData::isPersistent);
    data.def("setPersistent", &BaseData::setPersistent, sofapython3::doc::baseData::setPersistent);
    data.def("setParent", setParent, sofapython3::doc::baseData::setParent);
    data.def("hasParent", hasParent, sofapython3::doc::baseData::hasParent);
    data.def("read", &BaseData::read, sofapython3::doc::baseData::read);
    data.def("updateIfDirty", updateIfDirty, sofapython3::doc::baseData::updateIfDirty);
    data.def("isDirty", isDirty, sofapython3::doc::baseData::isDirty);
    data.def("isReadOnly",&BaseData::isReadOnly, sofapython3::doc::baseData::isReadOnly);
    data.def("setReadOnly", &BaseData::setReadOnly, sofapython3::doc::baseData::setReadOnly);
    data.def("isRequired", &BaseData::isRequired, sofapython3::doc::baseData::isRequired);
    data.def("getValueVoidPtr", &BaseData::getValueVoidPtr, sofapython3::doc::baseData::getValueVoidPtr);
}

} /// namespace sofapython3
