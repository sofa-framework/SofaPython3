#ifndef SOFAPYTHON3_SOFA_CORE_DATAHELPER_H
#define SOFAPYTHON3_SOFA_CORE_DATAHELPER_H

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

class PYBIND11_EXPORT PythonTrampoline
{
protected:
    std::shared_ptr<PyObject> pyobject;
public:
    virtual ~PythonTrampoline();
    virtual void PYBIND11_EXPORT setInstance(py::object s);
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

void PYBIND11_EXPORT setItem2D(py::array a, py::slice slice, py::object o);
void PYBIND11_EXPORT setItem2D(py::array a, const py::slice& slice,
               const py::slice& slice1, py::object o);
void PYBIND11_EXPORT setItem1D(py::array a, py::slice slice, py::object o);
void PYBIND11_EXPORT setItem(py::array a, py::slice slice, py::object value);

py::slice PYBIND11_EXPORT toSlice(const py::object& o);
std::string getPathTo(Base* b);
const char* getFormat(const AbstractTypeInfo& nfo);

std::map<void*, py::array>& getObjectCache();
void trimCache();
py::buffer_info PYBIND11_EXPORT toBufferInfo(BaseData& m);
py::object PYBIND11_EXPORT convertToPython(BaseData* d);

py::object PYBIND11_EXPORT toPython(BaseData* d, bool writeable=false);
void PYBIND11_EXPORT copyFromListScalar(BaseData& d, const AbstractTypeInfo& nfo, const py::list& l);
void PYBIND11_EXPORT fromPython(BaseData* d, const py::object& o);

template<typename T>
void PYBIND11_EXPORT copyScalar(BaseData* a, const AbstractTypeInfo& nfo, py::array_t<T, py::array::c_style> src)
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


class scoped_write_access
{
public:
    BaseData* data{nullptr};
    void* ptr{nullptr};
    scoped_write_access(BaseData* data_) : data(data_){ ptr = data->beginEditVoidPtr(); }
    ~scoped_write_access(){ data->endEditVoidPtr(); }
};

class scoped_read_access
{
public:
    BaseData* data{nullptr};
    void* ptr{nullptr};
    scoped_read_access(BaseData* data_) : data(data_){ ptr = data->beginEditVoidPtr(); }
    ~scoped_read_access(){ data->endEditVoidPtr(); }
};

class scoped_writeonly_access
{
public:
    BaseData* data {nullptr};
    void* ptr{nullptr};
    scoped_writeonly_access(BaseData* data_) : data(data_){ ptr = data->beginEditVoidPtr(); }
    ~scoped_writeonly_access(){ data->endEditVoidPtr(); }
};

}  // namespace sofapython3


#endif /// SOFAPYTHON3_SOFA_CORE_DATAHELPER_H


