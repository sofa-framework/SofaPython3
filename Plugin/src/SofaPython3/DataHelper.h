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

#pragma once

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <sofa/core/sptr.h>
#include <sofa/helper/Factory.h>
#include <sofa/core/objectmodel/Data.h>
#include <sofa/core/objectmodel/Base.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/objectmodel/BaseNode.h>

#include "config.h"

////////////////////////// FORWARD DECLARATION ///////////////////////////
namespace sofa {
    namespace defaulttype {
        class AbstractTypeInfo;
    }
    namespace core {
        namespace objectmodel {
            class BaseData;


            class SOFAPYTHON3_API PrefabLink
            {
            public:
                PrefabLink() {}
                PrefabLink(const Base::SPtr& targetBase) { m_targetBase = targetBase; }
                PrefabLink(BaseLink* targetLink) { m_targetBase = targetLink->getLinkedBase(); }
                PrefabLink(const std::string& targetPath) { m_targetPath = targetPath; }

                const Base::SPtr& getTargetBase() const { return m_targetBase; }
                void setTargetBase(const Base::SPtr& targetBase) { m_targetBase = targetBase; }

                const std::string& getTargetPath() const { return m_targetPath; }
                void setTargetPath(const std::string& targetPath) { m_targetPath = targetPath; }

                friend std::ostream& operator << ( std::ostream& out, const PrefabLink& l)
                {
                    if (l.getTargetBase())
                    {
                        auto bn = l.getTargetBase()->toBaseNode();
                        auto bo = l.getTargetBase()->toBaseObject();
                        out << "@" + (bn ? bn->getPathName() : bo->getPathName());
                    }
                    out << l.getTargetPath();
                    return out;
                }

                friend std::istream& operator >> ( std::istream& in, PrefabLink& l)
                {
                    std::string s;
                    in >> s;
                    l.setTargetPath(s);
                    return in;
                }

            private:
                Base::SPtr m_targetBase { nullptr };
                std::string m_targetPath {""};
            };
        }
    }
}

/////////////////////////////// DECLARATION //////////////////////////////
namespace sofapython3
{
namespace py { using namespace pybind11; }

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseData;
using sofa::core::objectmodel::BaseLink;
using sofa::core::objectmodel::BaseNode;
using sofa::core::objectmodel::BaseObject;
using sofa::defaulttype::AbstractTypeInfo;


class SOFAPYTHON3_API PythonTrampoline
{
protected:
    std::shared_ptr<PyObject> pyobject;
public:
    virtual ~PythonTrampoline();
    virtual void  setInstance(py::object s);
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

SOFAPYTHON3_API void setItem2D(py::array a, py::slice slice, py::object o);
SOFAPYTHON3_API void setItem2D(py::array a, const py::slice& slice,
               const py::slice& slice1, py::object o);
SOFAPYTHON3_API void setItem1D(py::array a, py::slice slice, py::object o);
SOFAPYTHON3_API void setItem(py::array a, py::slice slice, py::object value);

SOFAPYTHON3_API py::slice toSlice(const py::object& o);
SOFAPYTHON3_API std::string getPathTo(Base* b);
SOFAPYTHON3_API const char* getFormat(const AbstractTypeInfo& nfo);

SOFAPYTHON3_API std::map<void*, py::array>& getObjectCache();
SOFAPYTHON3_API void trimCache();

SOFAPYTHON3_API bool hasArrayFor(BaseData* d);
SOFAPYTHON3_API py::array resetArrayFor(BaseData* d);
SOFAPYTHON3_API py::array getPythonArrayFor(BaseData* d);



py::buffer_info SOFAPYTHON3_API toBufferInfo(BaseData& m);
py::object SOFAPYTHON3_API convertToPython(BaseData* d);

void SOFAPYTHON3_API copyFromListScalar(BaseData& d, const AbstractTypeInfo& nfo, const py::list& l);

std::string SOFAPYTHON3_API toSofaParsableString(const py::handle& p);

//py::object SOFAPYTHON3_API dataToPython(BaseData* d);

/// RVO optimized function. Don't care about copy on the return code.
py::list SOFAPYTHON3_API fillBaseObjectdescription(sofa::core::objectmodel::BaseObjectDescription& desc,
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

/// Following numpy convention returns the number of element in each dimmensions.
std::tuple<int, int> SOFAPYTHON3_API getShape(BaseData* self);

/// Following numpy convention the number of dimmension in the container.
size_t SOFAPYTHON3_API getNDim(BaseData* self);

/// Following numpy convention the number of elements in all the dimmension
/// https://docs.scipy.org/doc/numpy/reference/generated/numpy.ndarray.size.html#numpy.ndarray.size
size_t SOFAPYTHON3_API getSize(BaseData* self);

SOFAPYTHON3_API std::ostream& operator<<(std::ostream& out, const py::buffer_info& p);

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

SOFAPYTHON3_API BaseData* addData(py::object py_self, const std::string& name, py::object value = py::none(), py::object defaultValue = py::none(), const std::string& help = "", const std::string& group = "Property", std::string type = "");
SOFAPYTHON3_API BaseLink* addLink(py::object py_self, const std::string& name, py::object value, const std::string& help);
SOFAPYTHON3_API bool isProtectedKeyword(const std::string& name);

}  // namespace sofapython3


