#include <pybind11/numpy.h>
#include <pybind11/eval.h>
#include <pybind11/iostream.h>

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

void moduleAddBaseData(py::module& m)
{
    py::class_<BaseData, raw_ptr<BaseData>> data(m, "Data", sofapython3::doc::baseData::BaseDataClass);
    data.def("setName", &BaseData::setName, sofapython3::doc::baseData::setName);
    data.def("getName", &BaseData::getName, sofapython3::doc::baseData::getName);
    data.def("getCounter", [](BaseData& self) { return self.getCounter(); }, sofapython3::doc::baseData::getCounter);
    data.def("getHelp", &BaseData::getHelp, sofapython3::doc::baseData::getHelp);
    data.def("unset", &BaseData::unset, sofapython3::doc::baseData::unset);
    data.def("getOwner", &BaseData::getOwner, sofapython3::doc::baseData::getOwner);
    data.def("getParent", &BaseData::getParent, sofapython3::doc::baseData::getParent);
    data.def("typeName", [](BaseData& data){ return data.getValueTypeInfo()->name(); }, sofapython3::doc::baseData::typeName);
    data.def("getPathName", [](BaseData& self)
    {
        Base* b= self.getOwner();
        std::string prefix = getPathTo(b);
        return prefix+"."+self.getName();
    }, sofapython3::doc::baseData::getPathName);
    data.def("getLink", [](BaseData& self)
    {
        Base* b= self.getOwner();
        std::string prefix = getPathTo(b);
        return "@"+prefix+"."+self.getName();
    }, sofapython3::doc::baseData::getLink);

    data.def("hasChanged", [](BaseData& data){
        if (data.isDirty()) {
            data.update();
            return true;
        }
        return false;
    }, sofapython3::doc::baseData::hasChanged);

    data.def("isSet", [](BaseData& data){ return data.isSet(); }, sofapython3::doc::baseData::isSet);

    data.def("__str__", [](BaseData* self)
    {
        std::stringstream tmp;
        tmp << "Sofa.Core.Data <'" << self->getName() << "', " << self << ">";
        return py::str(tmp.str());
    });

    data.def("__repr__", [](BaseData* self)
    {
        return py::repr(toPython(self));
    });

    data.def("toList", [](BaseData* self){
        return convertToPython(self);
    }, sofapython3::doc::baseData::toList);

    data.def("array", [](BaseData* self){
        auto capsule = py::capsule(new Base::SPtr(self->getOwner()));
        py::buffer_info ninfo = toBufferInfo(*self);
        py::array a(pybind11::dtype(ninfo), ninfo.shape,
                    ninfo.strides, ninfo.ptr, capsule);
        a.attr("flags").attr("writeable") = false;
        return a;
    }, sofapython3::doc::baseData::array);

    data.def("writeableArray", [](BaseData* self, py::object f) -> py::object
    {
        if(self!=nullptr)
            return py::cast(new WriteAccessor(self, f));

        return py::none();
    }, sofapython3::doc::baseData::writeableArrayArg);

    data.def("writeableArray", [](BaseData* self) -> py::object
    {
        if(self!=nullptr)
            return py::cast(new WriteAccessor(self, py::none()));

        return py::none();
    }, sofapython3::doc::baseData::writeableArray);

    data.def("__setattr__", [](py::object self, const std::string& s, py::object value)
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
        if(s == "name"){
            py::cast<BaseData*>(self)->setName(py::cast<std::string>(value));
            return;
        }
        BindingBase::SetAttr(py::cast(selfdata->getOwner()),s,value);
    });

    data.def("__getattr__", [](py::object self, const std::string& s) -> py::object
    {
        /// If this is data.value we returns the content value of the data field converted into
        /// a python object that is easy to manipulate. The conversion is done with the toPython
        /// function.
        if(s == "value")
            return toPython(py::cast<BaseData*>(self));

        if(s == "name")
            return py::cast(py::cast<BaseData*>(self)->getName());
        /// BaseData does not support dynamic attributes, if you think this is an important feature
        /// please request for its integration.
        throw py::attribute_error("There is no attribute '"+s+"'");
    });

//    data.def("__mul__", [](py::object self, py::object d){
//        std::cout << "RETURN VALUE..." << std::endl;
//        return py::none();
//        //return toPython(py::cast<BaseData*>(self)).attr("__mul__")(f);
//    });

    data.def("getValueString",&BaseData::getValueString, sofapython3::doc::baseData::getValueString);
    data.def("getValueTypeString", &BaseData::getValueTypeString, sofapython3::doc::baseData::getValueTypeString);
    data.def("isPersistent", &BaseData::isPersistent, sofapython3::doc::baseData::isPersistent);
    data.def("setPersistent", &BaseData::setPersistent, sofapython3::doc::baseData::setPersistent);
    data.def("setParent", [](BaseData* self, BaseData* parent, std::string path) {
        self->setParent(parent, path);
    }, sofapython3::doc::baseData::setParent);
    data.def("hasParent", [](BaseData *self){
        return !self->getLinkPath().empty();
    }, sofapython3::doc::baseData::hasParent);
    data.def("getLinkPath", [](BaseData *self){
        return self->getLinkPath();
    }, sofapython3::doc::baseData::getLinkPath);
    data.def("getAsACreateObjectParameter", [](BaseData *self){
        return self->getLinkPath();
    }, sofapython3::doc::baseData::getAsACreateObjectParameter);
    data.def("read", &BaseData::read, sofapython3::doc::baseData::read);
    data.def("updateIfDirty", [](BaseData* self){
        return self->updateIfDirty();
    }, sofapython3::doc::baseData::updateIfDirty);
    data.def("isDirty", [](BaseData* self){
        return self->isDirty();
    }, sofapython3::doc::baseData::isDirty);
    data.def("isReadOnly",&BaseData::isReadOnly, sofapython3::doc::baseData::isReadOnly);
    data.def("setReadOnly", &BaseData::setReadOnly, sofapython3::doc::baseData::setReadOnly);
    data.def("isRequired", &BaseData::isRequired, sofapython3::doc::baseData::isRequired);

    data.def("getValueVoidPtr", &BaseData::getValueVoidPtr, sofapython3::doc::baseData::getValueVoidPtr);
}

} /// namespace sofapython3
