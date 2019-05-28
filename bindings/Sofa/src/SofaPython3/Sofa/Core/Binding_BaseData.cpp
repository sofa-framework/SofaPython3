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
#include "Binding_DataContainer.h"
#include "DataHelper.h"

namespace sofapython3
{

void moduleAddBaseData(py::module& m)
{
    py::class_<BaseData, raw_ptr<BaseData>> data(m, "Data");
    data.def("setName", &BaseData::setName);
    data.def("getName", &BaseData::getName);
    data.def("getCounter", &BaseData::getCounter );
    data.def("getHelp", &BaseData::getHelp);
    data.def("unset", &BaseData::unset);
    data.def("getOwner", &BaseData::getOwner);
    data.def("typeName", [](BaseData& data){ return data.getValueTypeInfo()->name(); });

    // TODO: Implementation should look like: https://github.com/sofa-framework/sofa/issues/767
//    p.def("__setitem__", [](BaseData& self, py::object& key, py::object& value)
//    {
//        std::cout << "mapping protocol, __setitem__ to implement)" << std::endl ;
//        return py::none();
//    });

//    p.def("__len__", [](BaseData& b) -> size_t
//    {
//        auto nfo = b.getValueTypeInfo();
//        return nfo->size(b.getValueVoidPtr()) / nfo->size();
//    });

    data.def("getPathName", [](BaseData& self)
    {
        Base* b= self.getOwner();
        std::string prefix = getPathTo(b);
        return prefix+"."+self.getName();
    });

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
    });

    data.def("array", [](BaseData* self){
        auto capsule = py::capsule(new Base::SPtr(self->getOwner()));
        py::buffer_info ninfo = toBufferInfo(*self);
        py::array a(pybind11::dtype(ninfo), ninfo.shape,
                    ninfo.strides, ninfo.ptr, capsule);
        a.attr("flags").attr("writeable") = false;
        return a;
    });

    data.def("writeableArray", [](BaseData* self, py::object f) -> py::object
    {
        if(self!=nullptr)
            return py::cast(new WriteAccessor(self, f));

        return py::none();
    });

    data.def("writeableArray", [](BaseData* self) -> py::object
    {
        if(self!=nullptr)
            return py::cast(new WriteAccessor(self, py::none()));

        return py::none();
    });

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

        BindingBase::SetAttr(py::cast(selfdata->getOwner()),s,value,true);
    });

    data.def("__getattr__", [](py::object self, const std::string& s) -> py::object
    {
        /// If this is data.value we returns the content value of the data field converted into
        /// a python object that is easy to manipulate. The conversion is done with the toPython
        /// function.
        if(s == "value")
            return toPython(py::cast<BaseData*>(self));

        /// BaseData does not support dynamic attributes, if you think this is an important feature
        /// please request for its integration.
        throw py::attribute_error("There is no attribute '"+s+"'");
    });

}

} /// namespace sofapython3
