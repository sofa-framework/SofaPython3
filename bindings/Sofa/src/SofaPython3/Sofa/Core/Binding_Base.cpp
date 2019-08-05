
#include <pybind11/pybind11.h>

#include <pybind11/numpy.h>

#include <sofa/core/topology/Topology.h>

#include <sofa/core/objectmodel/BaseData.h>
using sofa::core::objectmodel::BaseData;

#include <sofa/core/objectmodel/BaseLink.h>
using sofa::core::objectmodel::BaseLink;

#include <sofa/helper/accessor.h>
using sofa::helper::WriteOnlyAccessor;

#include <SofaPython3/PythonFactory.h>
//#include <SofaPython3/DataFactory.h>

#include "Binding_Base.h"
#include "Binding_Base_doc.h"


#include "Binding_DataDict.h"
#include "Binding_BaseData.h"
#include "Data/Binding_DataContainer.h"

#include <sofa/simulation/Node.h>
using sofa::simulation::Node;

#include <sofa/core/objectmodel/BaseObject.h>
using sofa::core::objectmodel::BaseObject;
#include <sofa/core/objectmodel/BaseNode.h>
using sofa::core::objectmodel::BaseNode;
#include <sofa/core/objectmodel/BaseContext.h>
using sofa::core::objectmodel::BaseContext;

#include <SofaPython3/DataHelper.h>

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
        return py::cast(l->getLinkedBase());

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

    /// We go for the container path.
    if(nfo.Container())
    {
        PythonFactory::fromPython(d,value);
        return true;
    }
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

BaseData* deriveTypeFromParent(BaseData* parentData)
{
    BaseData* newData = parentData->getNewInstance();
    newData->setParent(parentData);
    newData->update();
    return newData;
}

BaseData* deriveTypeFromParent(BaseContext* ctx, const std::string& link)
{
    if (!ctx)
        return nullptr;

    // if data is a link
    if (link.empty() || link[0] != '@')
        return nullptr;
    Base* component = ctx->toBaseNode();

    size_t pos = link.find_last_of('.');
    std::string componentPath = link.substr(0, pos);
    std::string parentDataName = link.substr(pos + 1);

    component = component->toBaseContext()->get<Base>(componentPath.substr(1));

    if (!component)
        throw py::value_error("No datafield found with path " + link);
    BaseData* parentData = component->findData(parentDataName);
    if (!parentData)
        throw py::value_error("No datafield found with path " + link);
    return deriveTypeFromParent(parentData);
}

bool isProtectedKeyword(const std::string& name)
{
    if (name == "children" || name == "objects" || name == "parents" ||
            name == "data" || name == "links")
    {
        return true;
    }
    return false;
}

void checkAmbiguousCreation(BaseNode* self, const std::string& name, const std::string& type)
{
    if (!self) return;

    if (type != "link")
        for (const auto& link : self->getLinks())
            if (link->getName() == name)
                msg_warning(self) << "Ambiguous creation of " << type << " named '" << name << "' in " << self->getPathName() << ": Component alread has a link with such name";

    if (type != "data")
        for (const auto& datafield : self->getDataFields())
            if (datafield->getName() == name)
                msg_warning(self) << "Ambiguous creation of " << type << " named '" << name << "' in " << self->getPathName() << ": Component alread has a data field with such name";

    if (type != "object")
        for (const auto& o : dynamic_cast<Node*>(self)->object)
            if (o->getName() == name)
                msg_warning(self) << "Ambiguous creation of " << type << " named '" << name << "' in " << self->getPathName() << ": Component alread has an object with such name";

    if (type != "child")
        for (const auto& child : self->getChildren())
            if (child->getName() == name)
                msg_warning(self) << "Ambiguous creation of " << type << " named '" << name << "' in " << self->getPathName() << ": Component alread has a child node with such name";

}

void checkAmbiguousCreation(BaseObject* self, const std::string& name, const std::string& type)
{
    if (!self) return;

    if (type != "link")
        for (const auto& link : self->getLinks())
            if (link->getName() == name)
                msg_warning(self) << "Ambiguous creation of " << type << " named '" << name << "' in " << self->getPathName() << ": Component alread has a link with such name";

    if (type != "data")
        for (const auto& datafield : self->getDataFields())
            if (datafield->getName() == name)
                msg_warning(self) << "Ambiguous creation of " << type << " named '" << name << "' in " << self->getPathName() << ": Component alread has a data field with such name";
}

void checkAmbiguousCreation(Base* self, const std::string& name, const std::string& type)
{
    checkAmbiguousCreation(dynamic_cast<BaseNode*>(self), name, type);
    checkAmbiguousCreation(dynamic_cast<BaseObject*>(self), name, type);
}

void checkAmbiguousCreation(py::object& py_self, const std::string& name, const std::string& type)
{
    Base* self = py::cast<Base*>(py_self);
    checkAmbiguousCreation(dynamic_cast<BaseNode*>(self), name, type);
    checkAmbiguousCreation(dynamic_cast<BaseObject*>(self), name, type);

    if (py_self.attr("__dict__").contains(name))
        msg_warning(self) << "Ambiguous creation of " << type << " named '" << name << "' in " << self->getName() << ": Component alread has a python attribute with such name in __dict__";
}

void BindingBase::addData(py::object py_self, const std::string& name, py::object value, py::object defaultValue, const std::string& help, const std::string& group, std::string type)
{
    Base* self = py::cast<Base*>(py_self);
    if (isProtectedKeyword(name))
        throw py::value_error("addData: Cannot call addData with name " + name + ": Protected keyword");
    checkAmbiguousCreation(py_self, name, "data");
    BaseData* data;

    bool isSet = true;
    if (value.is(py::none()))
    {
        value = defaultValue;
        isSet = false;
    }


    // create the data from the link passed as a string to the object
    if (py::isinstance<py::str>(value) &&
            !py::cast<std::string>(value).empty() &&
            (py::cast<std::string>(value))[0] == '@')
    {
        if (dynamic_cast<BaseNode*>(self))
            data = deriveTypeFromParent(dynamic_cast<BaseNode*>(self)->getContext(),
                                        py::cast<py::str>(value));
        else
            data = deriveTypeFromParent(dynamic_cast<BaseObject*>(self)->getContext(),
                                        py::cast<py::str>(value));
        if (!data)
            throw py::type_error("Cannot deduce type from value");
        self->addData(data, name);
    }
    // create the data from another data (use as parent)
    else if (py::isinstance<BaseData>(value) || py::isinstance<BaseData*>(value))
    {
        data = deriveTypeFromParent(py::cast<BaseData*>(value));
        if (!data)
            throw py::type_error("Cannot deduce type from value");
        self->addData(data, name);
    }
    // create the data from the type given in `type` and fill it up
    else
    {
        data = PythonFactory::createInstance(type);
        if (!data)
        {
            sofa::helper::vector<std::string> validTypes;
            PythonFactory::uniqueKeys(std::back_inserter(validTypes));
            std::string typesString = "[";
            for (const auto& i : validTypes)
                typesString += i + ", ";
            typesString += "\b\b]";

            throw py::type_error(std::string("Invalid Type string: available types are\n") + typesString);
        }
        self->addData(data, name);
        PythonFactory::fromPython(data, value);
    }
    data->setName(name);
    data->setGroup(group.c_str());
    data->setHelp(help.c_str());
    data->setDisplayed(true);
    data->setPersistent(true);
    if (!isSet)
        data->unset();
}


void BindingBase::addDataFromData(Base* self, py::object d)
{
    BaseData* data = py::cast<BaseData*>(d);
    if (!data)
        throw py::type_error("Argument is not a Data!");

    if (data->getOwner() == nullptr)
        self->addData(data, data->getName());
    else
    {
        BaseData* newData = data->getNewInstance();
        newData->setOwner(self);
        newData->setParent(data);
        newData->setName(data->getName());
    }
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

py::object BindingBase::getData(Base& self, const std::string& s)
{
    BaseData* d = self.findData(s);
    if(d!=nullptr)
    {
        return py::cast(d);
    }
    return py::none();
}

void moduleAddBase(py::module &m)
{
    py::class_<Base, Base::SPtr> base(m, "Base", py::dynamic_attr(), doc::base::BaseClass);
    /// set & get the name as string. The alternative is to access the data field using
    /// obj.name.value = "aName"
    base.def("getName", [](Base& b){ return b.getName(); });
    base.def("setName", [](Base& b, const std::string& s){ b.setName(s); } );
    base.def("getClassName",&Base::getClassName, sofapython3::doc::base::getClassName);
    base.def("getTemplateName",&Base::getTemplateName, sofapython3::doc::base::getTemplateName);
    base.def("getClass", &Base::getClass, pybind11::return_value_policy::reference, sofapython3::doc::base::getClass);
    base.def("getDefinitionSourceFilePos", &Base::getDefinitionSourceFilePos, sofapython3::doc::base::getDefinitionSourceFilePos);
    base.def("getDefinitionSourceFileName", &Base::getDefinitionSourceFileName, sofapython3::doc::base::getDefinitionSourceFileName);
    base.def("getInstanciationSourceFilePos", &Base::getInstanciationSourceFilePos, sofapython3::doc::base::getInstanciationSourceFilePos);
    base.def("getInstanciationFileName", &Base::getInstanciationSourceFileName, sofapython3::doc::base::getInstanciationSourceFilePos);
    base.def("getDataFields", &Base::getDataFields, pybind11::return_value_policy::reference, sofapython3::doc::base::getDataFields);
    base.def("findLink", &Base::findLink, pybind11::return_value_policy::reference, sofapython3::doc::base::findLink);
    base.def("getLinks", &Base::getLinks, pybind11::return_value_policy::reference, sofapython3::doc::base::getLinks);
    base.def("addData", &BindingBase::addData, "name"_a, "value"_a = py::none(), "default"_a = py::none(), "help"_a = "", "group"_a = "", "type"_a = "", sofapython3::doc::base::addData);
    base.def("addData", &BindingBase::addDataFromData, sofapython3::doc::base::addDataInitialized);
    base.def("__getattr__", &BindingBase::__getattr__);
    base.def("__setattr__", &BindingBase::__setattr__);
    base.def("getData", &BindingBase::getData, sofapython3::doc::base::getData);
    base.def("findData", &Base::findData, pybind11::return_value_policy::reference, sofapython3::doc::base::findData);
    base.def("__dir__", &BindingBase::__dir__);
}

} /// namespace sofapython3
