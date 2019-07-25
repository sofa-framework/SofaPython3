
#include <sofa/core/objectmodel/BaseNode.h>
#include <sofa/core/objectmodel/BaseData.h>
#include <sofa/defaulttype/DataTypeInfo.h>

#include <SofaPython3/DataHelper.h>
#include <SofaPython3/DataCache.h>

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

BindingDataFactory* getBindingDataFactoryInstance(){
    static BindingDataFactory* s_localfactory = new BindingDataFactory();
    if (s_localfactory == nullptr)
    {
        s_localfactory = new BindingDataFactory();
    }
    return s_localfactory ;
}

PythonTrampoline::~PythonTrampoline(){}
void PythonTrampoline::setInstance(py::object s)
{
    s.inc_ref();

    // TODO(bruno-marques) ici ça crash dans SOFA.
    //--ref_counter;

    pyobject = std::shared_ptr<PyObject>( s.ptr(), [](PyObject* ob)
    {
            // runSofa Sofa/tests/pyfiles/ScriptController.py => CRASH
            // Py_DECREF(ob);
});
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
        size_t dim0 = nfo.size(d->getValueVoidPtr())/nfo.size();
        size_t dim1 = nfo.size();
        py::list list;
        for(size_t i=0;i<dim0;i++)
        {
            py::list list1;
            for(size_t j=0;j<dim1;j++)
            {
                if(nfo.Integer())
                    list1.append(nfo.getIntegerValue(d->getValueVoidPtr(),i*dim1+j));
                if(nfo.Scalar())
                    list1.append(nfo.getScalarValue(d->getValueVoidPtr(),i*dim1+j));
                if(nfo.Text())
                    list1.append(nfo.getTextValue(d->getValueVoidPtr(),0));
            }
            list.append(list1);
        }
        return std::move(list);
    }

    if(nfo.Integer())
        return py::cast(nfo.getIntegerValue(d->getValueVoidPtr(), 0));
    if(nfo.Text())
        return py::cast(d->getValueString());
    if(nfo.Scalar())
        return py::cast(nfo.getScalarValue(d->getValueVoidPtr(), 0));

    if (!getBindingDataFactoryInstance()->createObject(nfo.name(), d).is_none())
        return getBindingDataFactoryInstance()->createObject(nfo.name(), d);

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

/// Make a python version of our BaseData.
/// Downcast everything.
py::object dataToPython(BaseData* d)
{
    const AbstractTypeInfo& nfo{ *(d->getValueTypeInfo()) };
    /// In case the data is a container with a simple layout
    /// we can expose the field as a numpy.array (no copy)
    if(nfo.Container() && nfo.SimpleLayout())
    {
        return getBindingDataFactoryInstance()->createObject("DataContainer", d);
    }
    if(nfo.Container() && nfo.Text())
    {
        return getBindingDataFactoryInstance()->createObject("DataVectorString", d);
    }
    if(nfo.Text())
    {
        return getBindingDataFactoryInstance()->createObject("DataString", d);
    }

    return py::cast(d);
}

/// Make a python version of our BaseData.
/// If possible the data is exposed as a numpy.array to minmize copy and data conversion
py::object toPython(BaseData* d, bool writeable)
{
    const AbstractTypeInfo& nfo{ *(d->getValueTypeInfo()) };
    /// In case the data is a container with a simple layout
    /// we can expose the field as a numpy.array (no copy)
    if(nfo.Container() && nfo.SimpleLayout())
    {
        if(!writeable)
        {
            getPythonArrayFor(d);
            return getBindingDataFactoryInstance()->createObject("DataContainer", d);
        }
        return getPythonArrayFor(d);
    }

    /// If this is not the case we return the converted datas (copy)
    return convertToPython(d);
}


template<class SrcType>
void copyFromListOf(const AbstractTypeInfo& nfo, void* ptr, size_t index, py::object o);

template<> void copyFromListOf<double>(const AbstractTypeInfo& nfo, void* ptr, size_t index, py::object o)
{
    nfo.setScalarValue(ptr, index, py::cast<double>(o));
}

template<> void copyFromListOf<int>(const AbstractTypeInfo& nfo, void* ptr, size_t index, py::object o)
{
    nfo.setIntegerValue(ptr, index, py::cast<int>(o));
}

template<> void copyFromListOf<std::string>(const AbstractTypeInfo& nfo, void* ptr, size_t index, py::object o)
{
    nfo.setTextValue(ptr, index, py::cast<std::string>(o));
}


template<class DestType>
void copyFromListOf(BaseData& d, const AbstractTypeInfo& nfo, const py::list& l)
{
    /// Check if the data is a single dimmension or not.
    py::buffer_info dstinfo = toBufferInfo(d);

    if(dstinfo.ndim>2)
        throw py::index_error("Invalid number of dimension only 1 or 2 dimensions are supported).");

    if(dstinfo.ndim==1)
    {
        void* ptr = d.beginEditVoidPtr();
        if( (size_t)dstinfo.shape[0] != l.size())
            nfo.setSize(ptr, l.size());

        for(size_t i=0;i<l.size();++i)
        {
            copyFromListOf<DestType>(nfo, ptr, i, l[i]);
        }
        d.endEditVoidPtr();
        return;
    }
    void* ptr = d.beginEditVoidPtr();
    auto t=getShape(&d);
    if( (size_t)dstinfo.shape[0] != l.size())
    {
        if( !nfo.setSize(ptr, l.size()*nfo.size()) )
            throw std::runtime_error("Unable to resize vector to match list size. Is the data type resizable ?");

        /// Update the buffer info entry to take into account the change of size.
        dstinfo = toBufferInfo(d);
    }

    for(auto i=0;i<dstinfo.shape[0];++i)
    {
        py::list ll = l[i];
        for(auto j=0;j<dstinfo.shape[1];++j)
        {
            copyFromListOf<DestType>(nfo, ptr, i*dstinfo.shape[1]+j, ll[j]);
        }
    }

    d.endEditVoidPtr();
    return;
}

template<>
void copyFromListOf<std::string>(BaseData& d, const AbstractTypeInfo& nfo, const py::list& l)
{
    void* ptr = d.beginEditVoidPtr();
    if( (size_t)nfo.size() != l.size())
        nfo.setSize(ptr, l.size());

    for(size_t i=0;i<l.size();++i)
    {
        copyFromListOf<std::string>(nfo, ptr, i, l[i]);
    }
    d.endEditVoidPtr();
    return;
}


void fromPython(BaseData* d, const py::object& o)
{
    const AbstractTypeInfo& nfo{ *(d->getValueTypeInfo()) };

    if(!nfo.Container())
    {
        scoped_writeonly_access guard(d);
        if(nfo.Integer())
            nfo.setIntegerValue(guard.ptr, 0, py::cast<int>(o));
        else if(nfo.Text())
            nfo.setTextValue(guard.ptr, 0, py::cast<py::str>(o));
        else if(nfo.Scalar())
            nfo.setScalarValue(guard.ptr, 0, py::cast<double>(o));
        return ;
    }

    if(nfo.Integer())
        return copyFromListOf<int>(*d, nfo, o);

    if(nfo.Text())
        return copyFromListOf<std::string>(*d, nfo, o);

    if(nfo.Scalar())
        return copyFromListOf<double>(*d, nfo, o);

    std::stringstream s;
    s<< "binding problem, trying to set value for "
     << d->getName() << ", " << py::cast<std::string>(py::str(o));
    throw std::runtime_error(s.str());
}
}
