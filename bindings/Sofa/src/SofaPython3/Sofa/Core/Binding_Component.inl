/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2021 CNRS, University of Lille, INRIA                  *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/cast.h>
#include <sofa/core/visual/VisualParams.h>
#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_Component.h>
#include <SofaPython3/Sofa/Core/Binding_Component_doc.h>
#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/PythonEnvironment.h>

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3
{
using sofa::core::objectmodel::Event;
using sofa::core::objectmodel::BaseComponent;


inline BasetTrampoline::BasetTrampoline(BaseComponent* self)
    : m_componentSelf(self)
{
}

inline BasetTrampoline::~BasetTrampoline()
{
    if (m_cacheInitialized)
    {
        PythonEnvironment::gil acquire {"~BasetTrampoline"};
        m_methodCache.clear();
        m_pySelf = py::object();
    }
}

inline void BasetTrampoline::initializePythonCache()
{
    if (m_cacheInitialized)
        return;

    // py::cast on the BaseComponent* will find the most-derived registered
    // pybind11 type (e.g. the user's Python subclass), which is exactly what
    // we want — no need for static_cast<T*>(this) anymore.
    m_pySelf = py::cast(m_componentSelf);

    getCachedMethod("onEvent");
    m_cacheInitialized = true;
}

inline py::object BasetTrampoline::getCachedMethod(const std::string& methodName)
{
    auto it = m_methodCache.find(methodName);
    if (it != m_methodCache.end())
        return it->second;

    py::object method;
    if (py::hasattr(m_pySelf, methodName.c_str()))
    {
        py::object fct = m_pySelf.attr(methodName.c_str());
        if (PyCallable_Check(fct.ptr()))
            method = fct;
    }

    m_methodCache[methodName] = method;
    return method;
}

inline bool BasetTrampoline::callCachedMethod(const py::object& method, Event* event)
{
    if (m_componentSelf->f_printLog.getValue())
    {
        std::string eventStr = py::str(PythonFactory::toPython(event));
        msg_info(m_componentSelf) << "on" << event->getClassName() << " " << eventStr;
    }

    py::object result = method(PythonFactory::toPython(event));
    if (result.is_none())
        return false;

    return py::cast<bool>(result);
}

inline void BasetTrampoline::invalidateMethodCache(const std::string& methodName)
{
    if (!m_cacheInitialized)
        return;
    m_methodCache.erase(methodName);
}

inline std::string BasetTrampoline::trampoline_getClassName() const
{
    PythonEnvironment::gil acquire {"getClassName"};

    if (m_pySelf)
        return py::str(py::type::of(m_pySelf).attr("__name__"));

    // Fallback before cache is initialized: cast via BaseComponent*
    return py::str(py::type::of(py::cast(m_componentSelf)).attr("__name__"));
}

inline bool BasetTrampoline::callScriptMethod(
    const py::object& self, Event* event, const std::string& methodName)
{
    if (m_componentSelf->f_printLog.getValue())
    {
        std::string name = std::string("on") + event->getClassName();
        std::string eventStr = py::str(PythonFactory::toPython(event));
        msg_info(m_componentSelf) << name << " " << eventStr;
    }

    if (py::hasattr(self, methodName.c_str()))
    {
        py::object fct = self.attr(methodName.c_str());
        py::object result = fct(PythonFactory::toPython(event));
        if (result.is_none())
            return false;
        return py::cast<bool>(result);
    }
    return false;
}

inline void BasetTrampoline::trampoline_handleEvent(Event* event)
{
    PythonEnvironment::executePython(m_componentSelf, [this, event](){
        if (!m_cacheInitialized)
            initializePythonCache();

        std::string methodName = std::string("on") + event->getClassName();

        py::object method = getCachedMethod(methodName);
        if (!method)
            method = getCachedMethod("onEvent");

        if (method)
        {
            bool isHandled = callCachedMethod(method, event);
            if (isHandled)
                event->setHandled();
        }
    });
}


template<class T>
sofa::core::sptr<T> trampoline_init(py::args& /*args*/, py::kwargs& kwargs)
{
    auto c = sofa::core::sptr<T>(new T());
    c->f_listening.setValue(true);

    for (auto kv : kwargs)
    {
        std::string key = py::cast<std::string>(kv.first);
        py::object value = py::reinterpret_borrow<py::object>(kv.second);

        if (key == "name")
            c->setName(py::cast<std::string>(kv.second));
        try {
            BindingBase::SetAttr(*c, key, value);
        } catch (py::attribute_error& /*e*/) {
            // kwargs may be plain Python attributes unrelated to SOFA — ignore
        }
    }
    return c;
}

template<class T>
void trampoline_setattr(py::object self, const std::string& s, py::object value)
{
    if (s.rfind("on", 0) == 0 && PyCallable_Check(value.ptr()))
    {
        auto* trampoline = dynamic_cast<T*>(py::cast<BaseComponent*>(self));
        if (trampoline)
            trampoline->invalidateMethodCache(s);
    }
    BindingBase::__setattr__(self, s, value);
}

} // namespace sofapython3
