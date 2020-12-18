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
#include <SofaPython3/config.h>

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

                inline bool operator ==(const PrefabLink& value) const
                {
                    if (getTargetBase())
                    {
                        if (value.getTargetBase())
                            return getTargetBase() == value.getTargetBase();
                        else
                            return getTargetBase()->getPathName() == value.getTargetPath();
                    }
                    else
                    {
                        if (value.getTargetBase())
                            return getTargetPath() == value.getTargetBase()->getPathName();
                        else
                            return getTargetPath() == value.getTargetPath();
                    }
                }

                inline bool operator !=(const PrefabLink& value) const
                {
                    return !(*this == value);
                }

            private:
                Base::SPtr m_targetBase { nullptr };
                std::string m_targetPath {""};
            };

            class SOFAPYTHON3_API DataPrefabLink : public Data<PrefabLink>
            {
                typedef Data<PrefabLink> Inherit;

                DataPrefabLink( const std::string& helpMsg="", bool isDisplayed=true, bool isReadOnly=false )
                    : Inherit(helpMsg, isDisplayed, isReadOnly)
                {
                }

                DataPrefabLink( const std::string& value, const std::string& helpMsg="", bool isDisplayed=true, bool isReadOnly=false )
                    : Inherit(value, helpMsg, isDisplayed, isReadOnly)
                {
                }

                explicit DataPrefabLink(const BaseData::BaseInitData& init)
                    : Inherit(init)
                {
                }

                const PrefabLink& getValue() const
                {
                    updateIfDirty();
                    if (m_value.getValue().getTargetBase()) return m_value.getValue();

                    auto self = const_cast<DataPrefabLink*>(this);

                    Base* dst = nullptr;
                    this->getOwner()->findLinkDest(dst, self->m_value.getValue().getTargetPath(), nullptr);
                    if (dst) {
                        auto edit = self->m_value.beginEdit();
                        edit->setTargetBase(dst);
                        edit->setTargetPath("");
                        self->m_value.endEdit();
                    }
                    return m_value.getValue();
                }

                std::string getValueString() const
                {
                    const auto& ptr = getValue();
                    if (ptr.getTargetBase())
                    {
                        auto bn = ptr.getTargetBase()->toBaseNode();
                        auto bo = ptr.getTargetBase()->toBaseObject();
                        return "@" + (bn ? bn->getPathName() : bo->getPathName());
                    }
                    return ptr.getTargetPath();
                }


                bool read(const std::string& value)
                {
                    Base* dst;
                    auto data = m_value.beginEdit();
                    if (this->getOwner()->findLinkDest(dst, value, nullptr) && dst != nullptr)
                       data->setTargetBase(dst);
                    else {
                        data->setTargetBase(nullptr);
                        data->setTargetPath(value);
                    }
                    return true;
                }
            };

        }
    }
}

/////////////////////////////// DECLARATION //////////////////////////////
namespace sofapython3
{

using sofa::core::objectmodel::Base;
using sofa::core::objectmodel::BaseData;
using sofa::core::objectmodel::BaseLink;
using sofa::core::objectmodel::BaseNode;
using sofa::core::objectmodel::BaseObject;
using sofa::defaulttype::AbstractTypeInfo;

SOFAPYTHON3_API void setItem2D(pybind11::array a, pybind11::slice slice, pybind11::object o);
SOFAPYTHON3_API void setItem2D(pybind11::array a, const pybind11::slice& slice,
               const pybind11::slice& slice1, pybind11::object o);
SOFAPYTHON3_API void setItem1D(pybind11::array a, pybind11::slice slice, pybind11::object o);
SOFAPYTHON3_API void setItem(pybind11::array a, pybind11::slice slice, pybind11::object value);

SOFAPYTHON3_API pybind11::slice toSlice(const pybind11::object& o);
SOFAPYTHON3_API std::string getPathTo(Base* b);
SOFAPYTHON3_API const char* getFormat(const AbstractTypeInfo& nfo);

SOFAPYTHON3_API std::map<void*, pybind11::array>& getObjectCache();
SOFAPYTHON3_API void trimCache();

SOFAPYTHON3_API bool hasArrayFor(BaseData* d);
SOFAPYTHON3_API pybind11::array resetArrayFor(BaseData* d);
SOFAPYTHON3_API pybind11::array getPythonArrayFor(BaseData* d);



pybind11::buffer_info SOFAPYTHON3_API toBufferInfo(BaseData& m);
pybind11::object SOFAPYTHON3_API convertToPython(BaseData* d);

void SOFAPYTHON3_API copyFromListScalar(BaseData& d, const AbstractTypeInfo& nfo, const pybind11::list& l);

std::string SOFAPYTHON3_API toSofaParsableString(const pybind11::handle& p);

//pybind11::object SOFAPYTHON3_API dataToPython(BaseData* d);

/// RVO optimized function. Don't care about copy on the return code.
void SOFAPYTHON3_API fillBaseObjectdescription(sofa::core::objectmodel::BaseObjectDescription& desc,
                               const pybind11::dict& dict);

template<typename T>
void copyScalar(BaseData* a, const AbstractTypeInfo& nfo, pybind11::array_t<T, pybind11::array::c_style> src)
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

SOFAPYTHON3_API std::ostream& operator<<(std::ostream& out, const pybind11::buffer_info& p);

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

SOFAPYTHON3_API BaseData* addData(pybind11::object py_self, const std::string& name, pybind11::object value = pybind11::none(), pybind11::object defaultValue = pybind11::none(), const std::string& help = "", const std::string& group = "Property", std::string type = "");
SOFAPYTHON3_API BaseLink* addLink(pybind11::object py_self, const std::string& name, pybind11::object value, const std::string& help);
SOFAPYTHON3_API bool isProtectedKeyword(const std::string& name);

}  // namespace sofapython3


