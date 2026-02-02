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
#include <sofa/core/behavior/BaseController.h>
#include <unordered_map>
#include <string>

namespace sofapython3 {

/**
 * Empty controller shell that allows pybind11 to bind the init and reinit methods (since BaseController doesn't have
 * them)
 */
class Controller : public sofa::core::behavior::BaseController {
public:
    SOFA_CLASS(Controller, sofa::core::behavior::BaseController);
    void init() override {};
    void reinit() override {};
};

class Controller_Trampoline : public Controller
{
public:
    SOFA_CLASS(Controller_Trampoline, Controller);

    Controller_Trampoline();
    ~Controller_Trampoline() override;

    void init() override;
    void reinit() override;
    void draw(const sofa::core::visual::VisualParams* params) override;

    void handleEvent(sofa::core::objectmodel::Event* event) override;

    std::string getClassName() const override;

private:
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

    /// Cache of Python method objects, keyed by method name
    /// Stores the method object if it exists, or an empty object if checked and not found
    std::unordered_map<std::string, pybind11::object> m_methodCache;

    /// Flag indicating whether the fallback "onEvent" method exists
    bool m_hasOnEvent = false;

    /// Cached reference to the fallback "onEvent" method
    pybind11::object m_onEventMethod;

    /// Flag indicating whether the cache has been initialized
    bool m_cacheInitialized = false;
};

void moduleAddController(pybind11::module &m);

} /// namespace sofapython3

