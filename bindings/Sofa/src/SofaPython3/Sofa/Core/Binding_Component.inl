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

template<class T>
Trampoline_T<T>::~Trampoline_T()
{
    // Clean up Python objects while holding the GIL
    if (m_cacheInitialized)
    {
        PythonEnvironment::gil acquire {"~Component_Trampoline"};
        m_methodCache.clear();
        m_pySelf = py::object();
    }
}

template<class T>
void Trampoline_T<T>::initializePythonCache()
{
    if (m_cacheInitialized)
        return;

    // Must be called with GIL held
    m_pySelf = py::cast(dynamic_cast<T*>(this));

    // Pre-cache the fallback "onEvent" method via the standard cache path
    getCachedMethod("onEvent");

    m_cacheInitialized = true;
}

template<class T>
py::object Trampoline_T<T>::getCachedMethod(const std::string& methodName)
{
    // Must be called with GIL held and cache initialized

    // Check if we've already looked up this method
    auto it = m_methodCache.find(methodName);
    if (it != m_methodCache.end())
    {
        return it->second;
    }

    // First time looking up this method - check if it exists
    py::object method;
    if (py::hasattr(m_pySelf, methodName.c_str()))
    {
        py::object fct = m_pySelf.attr(methodName.c_str());
        if (PyCallable_Check(fct.ptr()))
        {
            method = fct;
        }
    }

    // Cache the result (even if empty, to avoid repeated hasattr checks)
    m_methodCache[methodName] = method;
    return method;
}

template<class T>
bool Trampoline_T<T>::callCachedMethod(const py::object& method, Event* event)
{
    auto thisT = dynamic_cast<T*>(this);

    // Must be called with GIL held
    if (thisT->f_printLog.getValue())
    {
        std::string eventStr = py::str(PythonFactory::toPython(event));
        msg_info(thisT) << "on" << event->getClassName() << " " << eventStr;
    }

    py::object result = method(PythonFactory::toPython(event));
    if (result.is_none())
        return false;

    return py::cast<bool>(result);
}

template<class T>
void Trampoline_T<T>::invalidateMethodCache(const std::string& methodName)
{
    if (!m_cacheInitialized)
        return;

    // Remove the entry so getCachedMethod will re-resolve it on next call
    m_methodCache.erase(methodName);
}

template<class T>
std::string Trampoline_T<T>::trampoline_getClassName() const
{
    PythonEnvironment::gil acquire {"getClassName"};

    if (m_pySelf)
    {
        return py::str(py::type::of(m_pySelf).attr("__name__"));
    }

    // Fallback for when cache isn't initialized yet
    return py::str(py::type::of(py::cast(this)).attr("__name__"));
}

/// If a method named "methodName" exists in the python controller,
/// methodName is called, with the Event's dict as argument
template<class T>
bool Trampoline_T<T>::callScriptMethod(
        const py::object& self, Event* event, const std::string & methodName)
{
    auto thisT = dynamic_cast<T*>(this);

    if(thisT->f_printLog.getValue())
    {
        std::string name = std::string("on")+event->getClassName();
        std::string eventStr = py::str(PythonFactory::toPython(event));
        msg_info(thisT) << name << " " << eventStr;
    }

    if( py::hasattr(self, methodName.c_str()) )
    {
        py::object fct = self.attr(methodName.c_str());
        py::object result = fct(PythonFactory::toPython(event));
        if(result.is_none())
            return false;

        return py::cast<bool>(result);
    }
    return false;
}

template<class T>
void Trampoline_T<T>::trampoline_handleEvent(Event* event)
{
    PythonEnvironment::executePython(dynamic_cast<T*>(this), [this, event](){
        // Ensure cache is initialized (in case init() wasn't called or
        // handleEvent is called before init)
        if (!m_cacheInitialized)
        {
            initializePythonCache();
        }

        // Build the event-specific method name (e.g., "onAnimateBeginEvent")
        std::string methodName = std::string("on") + event->getClassName();

        // Try the event-specific method first, then fall back to generic "onEvent"
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
sofa::core::sptr<T>  Trampoline_T<T>::_init_(pybind11::args& /*args*/, pybind11::kwargs& kwargs)
{
    auto c = sofa::core::sptr<T> (new T());
    c->f_listening.setValue(true);

    for(auto kv : kwargs)
    {
        std::string key = py::cast<std::string>(kv.first);
        py::object value = py::reinterpret_borrow<py::object>(kv.second);

        if( key == "name")
            c->setName(py::cast<std::string>(kv.second));
        try {
            BindingBase::SetAttr(*c, key, value);
        } catch (py::attribute_error& /*e*/) {
            /// kwargs are used to set datafields to their initial values,
            /// but they can also be used as simple python attributes, unrelated to SOFA.
            /// thus we catch & ignore the py::attribute_error thrown by SetAttr
        }
    }
    return c;
}

template<class T>
void Trampoline_T<T>::_setattr_(pybind11::object self, const std::string& s, pybind11::object value)
{
    // If the attribute starts with "on" and the new value is callable, invalidate the cached method
    if (s.rfind("on", 0) == 0 && PyCallable_Check(value.ptr()))
    {
        auto* trampoline = dynamic_cast<T*>(py::cast<BaseComponent*>(self));
        if (trampoline)
        {
            trampoline->invalidateMethodCache(s);
        }
    }

    // Delegate to the base class __setattr__
    BindingBase::__setattr__(self, s, value);
}

}
