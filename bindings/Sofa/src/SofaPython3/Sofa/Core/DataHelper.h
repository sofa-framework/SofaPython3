#pragma once

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <sofa/core/sptr.h>
#include <sofa/helper/Factory.h>
#include <sofa/core/objectmodel/Data.h>

////////////////////////// FORWARD DECLARATION ///////////////////////////
namespace sofa {
    namespace defaulttype {
        class AbstractTypeInfo;
    }
    namespace core {
        namespace objectmodel {
            class Base;
            class BaseData;
            class BaseNode;
            class BaseObject;
        }
    }
}

/////////////////////////////// DECLARATION //////////////////////////////
namespace sofapython3
{
namespace py { using namespace pybind11; }

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseData;
using sofa::core::objectmodel::BaseNode;
using sofa::core::objectmodel::BaseObject;
using sofa::defaulttype::AbstractTypeInfo;


template<typename DataType>
class TypeCreator : public sofa::helper::BaseCreator<sofa::core::objectmodel::BaseData*, sofa::core::objectmodel::BaseData*, sofa::core::objectmodel::BaseData*>
{
public:
    virtual DataType createInstance(
            sofa::core::objectmodel::BaseData* data) override
    {
        return dynamic_cast<DataType>(data);
    }
    virtual const std::type_info& type() override { return typeid(sofa::Data<DataType>);}
};

class BindingDataFactory : public sofa::helper::Factory< std::string, sofa::core::objectmodel::BaseData*, sofa::core::objectmodel::BaseData*, sofa::core::objectmodel::BaseData*>
{
public:
    ObjectPtr createObject(std::string key, sofa::core::objectmodel::BaseData* arg) {
        ObjectPtr object;
        Creator* creator;
        typename std::multimap<std::string, Creator*>::iterator it = registry.lower_bound(key);
        typename std::multimap<std::string, Creator*>::iterator end = registry.upper_bound(key);
        while (it != end)
        {
            creator = (*it).second;
            object = creator->createInstance(arg);
            if (object != nullptr)
            {
                return object;
            }
            ++it;
        }
        return nullptr;
    }

};

BindingDataFactory* getBindingDataFactoryInstance();

class PythonTrampoline
{
protected:
    std::shared_ptr<PyObject> pyobject;
public:
    virtual ~PythonTrampoline();
    virtual void setInstance(py::object s);
};

template <typename T> class py_shared_ptr : public sofa::core::sptr<T>
{
public:
    py_shared_ptr(T *ptr) : sofa::core::sptr<T>(ptr)
    {
        std::cout << "Hooking the python objects..." << std::endl ;
        auto nptr = dynamic_cast<PythonTrampoline*>(ptr);
        if(nptr)
            nptr->setInstance( py::cast(ptr) ) ;
    }
};

void setItem2D(py::array a, py::slice slice, py::object o);
void setItem2D(py::array a, const py::slice& slice,
               const py::slice& slice1, py::object o);
void setItem1D(py::array a, py::slice slice, py::object o);
void setItem(py::array a, py::slice slice, py::object value);

py::slice toSlice(const py::object& o);
std::string getPathTo(Base* b);
const char* getFormat(const AbstractTypeInfo& nfo);

py::buffer_info toBufferInfo(BaseData& m);
py::object convertToPython(BaseData* d);

py::object toPython(BaseData* d, bool writeable=false);
void copyFromListScalar(BaseData& d, const AbstractTypeInfo& nfo, const py::list& l);
void fromPython(BaseData* d, const py::object& o);

std::string toSofaParsableString(const py::handle& p);

/// RVO optimized function. Don't care about copy on the return code.
void fillBaseObjectdescription(sofa::core::objectmodel::BaseObjectDescription& desc,
                               const py::dict& dict);

template<typename T>
void copyScalar(BaseData* a, const AbstractTypeInfo& nfo, py::array_t<T, py::array::c_style> src)
{
    void* ptr = a->beginEditVoidPtr();

    auto r = src.unchecked();
    for (ssize_t i = 0; i < r.shape(0); i++)
    {
        for (ssize_t j = 0; j < r.shape(1); j++)
        {
            nfo.setScalarValue( ptr, i*r.shape(1)+j, r(i,j) );
        }
    }
    a->endEditVoidPtr();
}

std::ostream& operator<<(std::ostream& out, const py::buffer_info& p);

// TODO: move this somewhere else as we will probably need it in several other places.
template <class T> class raw_ptr
{
public:
    raw_ptr() : ptr(nullptr) {}
    raw_ptr(T* ptr) : ptr(ptr) {}
    raw_ptr(const raw_ptr& other) : ptr(other.ptr) {}
    T& operator* () const { return *ptr; }
    T* operator->() const { return  ptr; }
    T* get() const { return ptr; }
    void destroy() { delete ptr; }
    T& operator[](std::size_t idx) const { return ptr[idx]; }
private:
    T* ptr;
};

}  // namespace sofapython3


