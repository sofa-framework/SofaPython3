#include <pybind11/numpy.h>
#include <pybind11/eval.h>

#include <sofa/defaulttype/DataTypeInfo.h>
using sofa::defaulttype::AbstractTypeInfo;

#include <sofa/core/objectmodel/BaseData.h>
using sofa::core::objectmodel::BaseData;

#include <sofa/core/objectmodel/BaseObject.h>
using  sofa::core::objectmodel::BaseObject;

#include <sofa/core/objectmodel/BaseNode.h>
using  sofa::core::objectmodel::BaseNode;

#include "Binding_Base.h"
#include "Binding_BaseData.h"
#include "Data/Binding_DataContainer.h"
#include <SofaPython3/DataHelper.h>
#include "Binding_BaseData_doc.h"
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
    return py::repr(toPython(self));
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
        py::array a = getPythonArrayFor(selfdata);
        BindingBase::SetDataFromArray(selfdata, a);
        return;
    }

    if(py::isinstance<py::array>(value))
    {
        BindingBase::SetDataFromArray(selfdata, py::cast<py::array>(value));
        return;
    }
    BindingBase::SetAttr(py::cast(selfdata->getOwner()),s,value);
}

py::object __getattr__(py::object self, const std::string& s)
{
    /// If this is data.value we returns the content value of the data field converted into
    /// a python object that is easy to manipulate. The conversion is done with the toPython
    /// function.
    if(s == "value")
        return toPython(py::cast<BaseData*>(self));

    /// BaseData does not support dynamic attributes, if you think this is an important feature
    /// please request for its integration.
    throw py::attribute_error("There is no attribute '"+s+"'");
}

void setParent(BaseData* self, BaseData* parent, std::string path)
{
    self->setParent(parent, path);
}

bool hasParent(BaseData *self)
{
    return !self->getLinkPath().empty();
}

py::str getAsACreateObjectParameter(BaseData *self)
{
    return self->getLinkPath();
}

void updateIfDirty(BaseData* self)
{
    return self->updateIfDirty();
}

bool isDirty(BaseData* self)
{
    return self->isDirty();
}

void moduleAddBaseData(py::module& m)
{
    /// Register the BaseData binding into the pybind11 system.
    py::class_<BaseData, raw_ptr<BaseData>> data(m, "Data", sofapython3::doc::baseData::BaseDataClass);
    data.def("getName", [](BaseData& b){ return b.getName(); });
    data.def("setName", [](BaseData& b, const std::string& s){ b.setName(s); } );
    data.def("getCounter", [](BaseData& self) { return self.getCounter(); }, sofapython3::doc::baseData::getCounter);
    data.def("getHelp", &BaseData::getHelp, sofapython3::doc::baseData::getHelp);
    data.def("unset", &BaseData::unset, sofapython3::doc::baseData::unset);
    data.def("getOwner", &BaseData::getOwner, sofapython3::doc::baseData::getOwner);
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
    data.def("getAsACreateObjectParameter", getAsACreateObjectParameter, sofapython3::doc::baseData::getAsACreateObjectParameter);
    data.def("read", &BaseData::read, sofapython3::doc::baseData::read);
    data.def("updateIfDirty", updateIfDirty, sofapython3::doc::baseData::updateIfDirty);
    data.def("isDirty", isDirty, sofapython3::doc::baseData::isDirty);
    data.def("isReadOnly",&BaseData::isReadOnly, sofapython3::doc::baseData::isReadOnly);
    data.def("setReadOnly", &BaseData::setReadOnly, sofapython3::doc::baseData::setReadOnly);
    data.def("isRequired", &BaseData::isRequired, sofapython3::doc::baseData::isRequired);
    data.def("getValueVoidPtr", &BaseData::getValueVoidPtr, sofapython3::doc::baseData::getValueVoidPtr);
}

} /// namespace sofapython3
