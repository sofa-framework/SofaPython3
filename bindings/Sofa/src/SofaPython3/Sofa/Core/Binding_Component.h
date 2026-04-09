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
#include <sofa/core/objectmodel/BaseComponent.h>
#include <string>
#include <unordered_map>

namespace sofapython3 {


template<class T>
class Trampoline_T
{
public:

    virtual ~Trampoline_T();


    void trampoline_handleEvent(sofa::core::objectmodel::Event* event);

    std::string trampoline_getClassName() const;

    /// Invalidates a specific entry in the method cache (called when a user reassigns an on* attribute)
    void invalidateMethodCache(const std::string& methodName);

    static sofa::core::sptr<T> _init_(pybind11::args& /*args*/, pybind11::kwargs& kwargs);

    static void _setattr_(pybind11::object self, const std::string& s, pybind11::object value);

protected:
    /// Initializes the Python object cache (m_pySelf and method cache)
    void initializePythonCache();

    /// Returns a cached method if it exists, or an empty object if not
    pybind11::object getCachedMethod(const std::string& methodName);

    /// Calls a cached Python method with the given event
    bool callCachedMethod(const pybind11::object& method, sofa::core::objectmodel::Event* event);

    /// Legacy method for uncached calls (fallback)
    bool callScriptMethod(const pybind11::object& self, sofa::core::objectmodel::Event* event,
        const std::string& methodName);

    /// Cached Python self reference (avoids repeated py::cast(this))
    pybind11::object m_pySelf;

    /// Cache of Python method objects, keyed by method name (including "onEvent" fallback)
    /// Stores the method object if it exists, or an empty object if checked and not found
    std::unordered_map<std::string, pybind11::object> m_methodCache;

    /// Flag indicating whether the cache has been initialized
    bool m_cacheInitialized = false;
};


/**
 * Empty controller shell that allows pybind11 to bind the init and reinit methods (since BaseComponent doesn't have
 * them)
 */
class Component : public sofa::core::objectmodel::BaseComponent {
public:
    SOFA_CLASS(Component, sofa::core::objectmodel::BaseComponent);
    void init() override {};
    void reinit() override {};
};

class Component_Trampoline : public Component, public Trampoline_T<Component_Trampoline>
{
public:
    SOFA_CLASS(Component_Trampoline, Component);

    Component_Trampoline() = default;
    virtual ~Component_Trampoline() = default;

    void init() override;
    void reinit() override;
    void draw(const sofa::core::visual::VisualParams* params) override;

    void handleEvent(sofa::core::objectmodel::Event* event) override;

    std::string getClassName() const override;

};

void moduleAddComponent(pybind11::module &m);

} /// namespace sofapython3

