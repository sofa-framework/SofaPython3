#pragma once

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <sofa/core/sptr.h>
#include <sofa/helper/Factory.h>
#include <sofa/core/objectmodel/Data.h>

#include "config.h"

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


class SOFAPYTHON3_API PythonTrampoline
{
protected:
    std::shared_ptr<PyObject> pyobject;
public:
    virtual ~PythonTrampoline();
    virtual void SOFAPYTHON3_API setInstance(py::object s);
};

template <typename T> class py_shared_ptr : public sofa::core::sptr<T>
{
public:
    py_shared_ptr(T *ptr) : sofa::core::sptr<T>(ptr)
    {
        auto nptr = dynamic_cast<PythonTrampoline*>(ptr);
        if(nptr)
            nptr->setInstance( py::cast(ptr) ) ;
    }
};

void SOFAPYTHON3_API setItem2D(py::array a, py::slice slice, py::object o);
void SOFAPYTHON3_API setItem2D(py::array a, const py::slice& slice,
               const py::slice& slice1, py::object o);
void SOFAPYTHON3_API setItem1D(py::array a, py::slice slice, py::object o);
void SOFAPYTHON3_API setItem(py::array a, py::slice slice, py::object value);

py::slice SOFAPYTHON3_API toSlice(const py::object& o);
std::string getPathTo(Base* b);
const char* getFormat(const AbstractTypeInfo& nfo);

std::map<void*, py::array>& SOFAPYTHON3_API getObjectCache();
void trimCache();

bool hasArrayFor(BaseData* d);
py::array SOFAPYTHON3_API resetArrayFor(BaseData* d);
py::array SOFAPYTHON3_API getPythonArrayFor(BaseData* d);



py::buffer_info SOFAPYTHON3_API toBufferInfo(BaseData& m);
py::object SOFAPYTHON3_API convertToPython(BaseData* d);

void SOFAPYTHON3_API copyFromListScalar(BaseData& d, const AbstractTypeInfo& nfo, const py::list& l);

std::string SOFAPYTHON3_API toSofaParsableString(const py::handle& p);

//py::object SOFAPYTHON3_API dataToPython(BaseData* d);

/// RVO optimized function. Don't care about copy on the return code.
void SOFAPYTHON3_API fillBaseObjectdescription(sofa::core::objectmodel::BaseObjectDescription& desc,
                               const py::dict& dict);

template<typename T>
void SOFAPYTHON3_API copyScalar(BaseData* a, const AbstractTypeInfo& nfo, py::array_t<T, py::array::c_style> src)
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

/// Following numpy convention returns the number of element in each dimmensions.
std::tuple<int, int> getShape(BaseData* self);

/// Following numpy convention the number of dimmension in the container.
size_t SOFAPYTHON3_API getNDim(BaseData* self);

/// Following numpy convention the number of elements in all the dimmension
/// https://docs.scipy.org/doc/numpy/reference/generated/numpy.ndarray.size.html#numpy.ndarray.size
size_t SOFAPYTHON3_API getSize(BaseData* self);

std::ostream& SOFAPYTHON3_API operator<<(std::ostream& out, const py::buffer_info& p);

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


