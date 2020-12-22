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


#include <sofa/core/objectmodel/BaseNode.h>
#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/defaulttype/DataTypeInfo.h>
#include <sofa/simulation/Node.h>

#include <SofaPython3/DataHelper.h>
#include <SofaPython3/DataCache.h>
#include <SofaPython3/PythonFactory.h>

using sofa::core::objectmodel::BaseLink;

namespace sofapython3
{

std::string toSofaParsableString(const py::handle& p)
{
    if(py::isinstance<py::list>(p) || py::isinstance<py::tuple>(p))
    {
        std::stringstream tmp;
        for(auto pa : p){
            tmp << toSofaParsableString(pa) << " ";
        }
        return tmp.str();
    }
    //TODO(dmarchal) This conversion to string is so bad.
    if(py::isinstance<py::str>(p))
        return py::str(p);
    return py::repr(p);
}

/// RVO optimized function. Don't care about copy on the return code.
void fillBaseObjectdescription(sofa::core::objectmodel::BaseObjectDescription& desc,
                               const py::dict& dict)
{
    for(auto kv : dict)
    {
        desc.setAttribute(py::str(kv.first), toSofaParsableString(kv.second));
    }
}

std::ostream& operator<<(std::ostream& out, const py::buffer_info& p)
{
    out << "buffer{"<< p.format << ", " << p.ndim << ", " << p.shape[0];
    if(p.ndim==2)
        out << ", " << p.shape[1];
    out << ", " << p.size << "}";
    return out;
}

std::string getPathTo(Base* b)
{
    BaseNode* node = dynamic_cast<BaseNode*>(b);
    if(node)
        return node->getPathName();
    BaseObject* object = dynamic_cast<BaseObject*>(b);
    if(object)
        return object->getPathName();

    assert(true && "Only Base & BaseObject are supported");
    return "";
}

const char* getFormat(const AbstractTypeInfo& nfo)
{
    if(nfo.Integer())
    {
        return py::format_descriptor<long>::value;
    } else if(nfo.Scalar() )
    {
        if(nfo.byteSize() == 8)
            return py::format_descriptor<double>::value;
        else if(nfo.byteSize() == 4)
            return py::format_descriptor<float>::value;
    }
    return nullptr;
}

template<class Array, typename Value>
void setValueArray1D(Array p,
                     const py::slice& slice,
                     const Value& v)
{
    size_t start, stop, step, slicelength;
    if (!slice.compute(p.shape(0), &start, &stop, &step, &slicelength))
        throw py::error_already_set();

    for (size_t i = 0; i < slicelength; ++i) {
        p(start) = v;
        start += step;
    }
}

template<class Array, typename Value>
void setValueArray2D(Array p,
                     const py::slice& slice,
                     const Value& v)
{
    size_t start, stop, step, slicelength;
    if (!slice.compute(p.shape(0), &start, &stop, &step, &slicelength))
        throw py::error_already_set();

    for (size_t i = 0; i < slicelength; ++i, start+=step) {
        for(size_t j=0; j<p.shape(1);++j){
            p(start, j) = v;
        }
    }
}

template<class Array, typename Value>
void setItem2DTyped(Array a, py::slice slice, Value dvalue)
{
    size_t start, stop, step, slicelength;
    if (!slice.compute(a.shape(0), &start, &stop, &step, &slicelength))
        throw py::error_already_set();

    for(size_t i=0;i<slicelength;++i, start+=step)
        for(int j=0;j<a.shape(1);++j)
            a(start, j) = dvalue;
}

template<class Array, typename Value>
void setItem2DTyped(Array a, py::slice sliceI, py::slice sliceJ, Value dvalue)
{
    size_t startI, stopI, stepI, slicelengthI;
    if (!sliceI.compute(a.shape(0), &startI, &stopI, &stepI, &slicelengthI))
        throw py::error_already_set();

    size_t startJ, stopJ, stepJ, slicelengthJ;
    if (!sliceJ.compute(a.shape(1), &startJ, &stopJ, &stepJ, &slicelengthJ))
        throw py::error_already_set();

    for(size_t i=0;i<slicelengthI;++i, startI+=stepI)
    {
        for(size_t j=0, itJ=startJ;j<slicelengthJ;++j, itJ+=stepJ)
        {
            a(startI, itJ) = dvalue;
        }
    }
}

void setItem2D(py::array a, py::slice slice, py::object o)
{
    if(a.request().format=="d")
        setItem2DTyped(a.mutable_unchecked<double, 2>(), slice, py::cast<double>(o));
    else if(a.request().format=="f")
        setItem2DTyped(a.mutable_unchecked<float, 2>(), slice, py::cast<float>(o));
    else
        throw py::type_error("Invalid type");
}

void setItem2D(py::array a, const py::slice& slice, const py::slice& slice1, py::object o)
{
    if(a.request().format=="d")
        setItem2DTyped(a.mutable_unchecked<double, 2>(), slice, slice1, py::cast<double>(o));
    else if(a.request().format=="f")
        setItem2DTyped(a.mutable_unchecked<float, 2>(), slice, slice1, py::cast<float>(o));
    else
        throw py::type_error("Invalid type");
}

template<class Array, typename Value>
void setItem1DTyped(Array a, py::slice slice, Value dvalue)
{
    size_t start, stop, step, slicelength;
    if (!slice.compute(a.shape(0), &start, &stop, &step, &slicelength))
        throw py::error_already_set();

    for(size_t i=0;i<slicelength;++i, start+=step)
        a(start) = dvalue;
}

void setItem1D(py::array a, py::slice slice, py::object o)
{
    if(a.request().format=="d")
        setItem1DTyped(a.mutable_unchecked<double, 1>(), slice, py::cast<double>(o));
    else if(a.request().format=="f")
        setItem1DTyped(a.mutable_unchecked<float, 1>(), slice, py::cast<float>(o));
    else
        throw py::type_error("Invalid type");
}

void setItem(py::array a, py::slice slice, py::object value)
{
    if(a.ndim()>2)
        throw py::index_error("DataContainer can only operate on 1 or 2D array.");

    else if(a.ndim()==1)
        setItem1D(a, slice, value);

    else if(a.ndim()==2)
        setItem2D(a, slice, value);
}

py::slice toSlice(const py::object& o)
{
    if( py::isinstance<py::slice>(o))
        return py::cast<py::slice>(o);

    size_t v = py::cast<size_t>(o);
    return py::slice(v,v+1,1);
}


std::map<void*, py::array>& getObjectCache()
{
    static std::map<void*, py::array>* s_objectcache {nullptr} ;
    if(!s_objectcache)
    {
        s_objectcache=new std::map<void*, py::array>();
    }
    return *s_objectcache;
}

void trimCache()
{
    auto& memcache = getObjectCache();
    if(memcache.size() > 1000)
    {
        std::cout << "flushing the cache (it is too late to implement LRU)" << std::endl ;
        memcache.clear();
    }
}


/// Following numpy convention returns the number of element in each dimmensions.
std::tuple<int, int> getShape(BaseData* self)
{
    /// Detect if we are in a one or two dimmension array.
    auto nfo = self->getValueTypeInfo();
    auto itemNfo = nfo->BaseType();

    /// If the data is a container and its "item" is not a container we are manipulating
    /// a 1D array.
    if( !itemNfo->Container() )
        return  {nfo->size(self->getValueVoidPtr())/itemNfo->size(), -1};
    return {nfo->size(self->getValueVoidPtr())/itemNfo->size(), itemNfo->size()};
}

/// Following numpy convention the number of dimmension in the container.
size_t getNDim(BaseData* self)
{
    auto nfo = self->getValueTypeInfo();
    auto itemNfo = nfo->BaseType();

    if( itemNfo->Container() )
        return 2;
    return 1;
}

/// Following numpy convention the number of elements in all the dimmension
/// https://docs.scipy.org/doc/numpy/reference/generated/numpy.ndarray.size.html#numpy.ndarray.size
size_t getSize(BaseData* self)
{
    auto nfo = self->getValueTypeInfo();
    return nfo->size(self->getValueVoidPtr());
}

py::buffer_info toBufferInfo(BaseData& m)
{
    const AbstractTypeInfo& nfo { *m.getValueTypeInfo() };
    auto itemNfo = nfo.BaseType();

    const char* format = nullptr;
    if(nfo.Integer())
    {
        if(nfo.byteSize() == 8)
            format = py::format_descriptor<int64_t>::value;
        else if(nfo.byteSize() == 4)
            format = py::format_descriptor<int32_t>::value;
        else if(nfo.byteSize() == 2)
            format = py::format_descriptor<int16_t>::value;
        else if(nfo.byteSize() == 1)
            format = py::format_descriptor<int8_t>::value;
    } else if(nfo.Scalar() )
    {
        if(nfo.byteSize() == 8)
            format = py::format_descriptor<double>::value;
        else if(nfo.byteSize() == 4)
            format = py::format_descriptor<float>::value;
    }

    size_t datasize = nfo.byteSize();

    std::tuple<int,int> shape = getShape(&m);
    size_t  ndim = getNDim(&m);

    void* ptr = const_cast<void*>(nfo.getValuePtr(m.getValueVoidPtr()));
    if( !itemNfo->Container() ){
        return py::buffer_info(
                    ptr, /* Pointer to buffer */
                    datasize,                              /* Size of one scalar */
                    format,                                /* Python struct-style format descriptor */
                    1,                                     /* Number of dimensions */
        { std::get<0>(shape) },                              /* Buffer dimensions */
        { datasize }                           /* Strides (in bytes) for each index */
                    );
    }
    py::buffer_info ninfo(
                ptr,  /* Pointer to buffer */
                datasize,                              /* Size of one scalar */
                format,                                /* Python struct-style format descriptor */
                2,                                     /* Number of dimensions */
    { std::get<0>(shape), std::get<1>(shape)},                        /* Buffer dimensions */
    { datasize * std::get<1>(shape) ,    datasize }                         /* Strides (in bytes) for each index */
                );
    return ninfo;
}

py::object convertToPython(BaseData* d)
{
    const AbstractTypeInfo& nfo{ *(d->getValueTypeInfo()) };
    if(hasArrayFor(d))
        return getPythonArrayFor(d);

    if(nfo.Container())
    {
        py::list list;
        if (nfo.Text())
        {
            for (size_t i = 0 ; i < nfo.size(d->getValueVoidPtr()) ; ++i)
                list.append(nfo.getTextValue(d->getValueVoidPtr(),i));
        }
        else
        {
            size_t dim0 = nfo.size(d->getValueVoidPtr())/nfo.size();
            size_t dim1 = nfo.size();
            for(size_t i=0;i<dim0;i++)
            {

                py::list list1;
                for(size_t j=0;j<dim1;j++)
                {
                    if(nfo.Integer())
                        list1.append(nfo.getIntegerValue(d->getValueVoidPtr(),i*dim1+j));
                    if(nfo.Scalar())
                        list1.append(nfo.getScalarValue(d->getValueVoidPtr(),i*dim1+j));
                }
                list.append(list1);
            }
        }
        return std::move(list);
    }

    if(nfo.Integer())
        return py::cast(nfo.getIntegerValue(d->getValueVoidPtr(), 0));
    if(nfo.Text())
        return py::cast(d->getValueString());
    if(nfo.Scalar())
        return py::cast(nfo.getScalarValue(d->getValueVoidPtr(), 0));

    return py::cast(d->getValueString());
}

bool hasArrayFor(BaseData* d)
{
    auto& memcache = getObjectCache();
    return memcache.find(d) != memcache.end();
}

py::array resetArrayFor(BaseData* d)
{
    //todo: protect the function.
    auto& memcache = getObjectCache();
    auto capsule = py::capsule(new Base::SPtr(d->getOwner()));

    py::buffer_info ninfo = toBufferInfo(*d);
    py::array a(pybind11::dtype(ninfo), ninfo.shape,
                ninfo.strides, ninfo.ptr, capsule);

    memcache[d] = a;
    return a;
}

py::array getPythonArrayFor(BaseData* d)
{
    auto& memcache = getObjectCache();
    if(memcache.find(d) == memcache.end())
    {
        auto capsule = py::capsule(new Base::SPtr(d->getOwner()));

        py::buffer_info ninfo = toBufferInfo(*d);
        py::array a(pybind11::dtype(ninfo), ninfo.shape,
                    ninfo.strides, ninfo.ptr, capsule);

        memcache[d] = a;
        return a;
    }
    return memcache[d];
}


BaseData* deriveTypeFromParent(BaseData* parentData)
{
    BaseData* newData = parentData->getNewInstance();
    newData->setParent(parentData);
    newData->update();
    return newData;
}

BaseData* deriveTypeFromParent(sofa::core::objectmodel::BaseContext* ctx, const std::string& link)
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
        for (const auto& o : dynamic_cast<sofa::simulation::Node*>(self)->object)
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


BaseData* addData(py::object py_self, const std::string& name, py::object value, py::object defaultValue, const std::string& help, const std::string& group, std::string type)
{
    Base* self = py::cast<Base*>(py_self);
    if (isProtectedKeyword(name))
        throw py::value_error("addData: Cannot call addData with name " + name + ": Protected keyword");
    checkAmbiguousCreation(py_self, name, "data");
    BaseData* data;

    bool isSet = true;
    if (value.is_none())
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
    else if (!value.is_none() && (py::isinstance<BaseData>(value) || py::isinstance<BaseData*>(value)))
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
        if (!value.is_none())
            PythonFactory::fromPython(data, value);
    }
    data->setName(name);
    data->setGroup(group.c_str());
    data->setHelp(help.c_str());
    data->setDisplayed(true);
    data->setPersistent(true);
    if (!isSet)
        data->unset();
    return data;
}

BaseLink* addLink(py::object py_self, const std::string& name, py::object value, const std::string& help)
{
    Base* self = py::cast<Base*>(py_self);
    if (isProtectedKeyword(name))
        throw py::value_error("addLink: Cannot call addLink with name " + name + ": Protected keyword");

    checkAmbiguousCreation(py_self, name, "link");

    BaseLink::InitLink<Base> initlink(self, name, help);

    BaseLink* link = new sofa::core::objectmodel::SingleLink<Base, Base, BaseLink::FLAG_MULTILINK>(initlink);
    if (py::isinstance<std::string>(value))
    {
        auto linkpath = py::cast<std::string>(value);
        if (linkpath[0] != '@')
            linkpath = "@" + linkpath;
        if (!link->read(linkpath))
            throw py::value_error("addLink: Cannot read link path " + linkpath + ": is link valid?");
    }
    else if (py::isinstance<Base*>(value))
        link->setLinkedBase(py::cast<Base*>(value));

//    self->addLink(link);
    return link;
}

}  // namespace sofapython3
