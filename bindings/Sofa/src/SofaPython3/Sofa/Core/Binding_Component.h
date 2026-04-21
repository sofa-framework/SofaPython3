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


class TrampolineBase
{
public:
    explicit TrampolineBase(sofa::core::objectmodel::BaseComponent* self);
    virtual ~TrampolineBase();

    void trampoline_handleEvent(sofa::core::objectmodel::Event* event);
    std::string trampoline_getClassName() const;
    void invalidateMethodCache(const std::string& methodName);

protected:
    void initializePythonCache();
    pybind11::object getCachedMethod(const std::string& methodName);
    bool callCachedMethod(const pybind11::object& method, sofa::core::objectmodel::Event* event);
    bool callScriptMethod(const pybind11::object& self, sofa::core::objectmodel::Event* event,
                          const std::string& methodName);

    /// Raw non-owning pointer to the concrete trampoline as BaseComponent.
    /// Safe because TrampolineBase is always embedded in the same object.
    sofa::core::objectmodel::BaseComponent* m_componentSelf { nullptr };

    pybind11::object m_pySelf;
    std::unordered_map<std::string, pybind11::object> m_methodCache;
    bool m_cacheInitialized = false;
};


template<class T>
sofa::core::sptr<T> trampoline_init(pybind11::args& /*args*/, pybind11::kwargs& kwargs);

template<class T>
void trampoline_setattr(pybind11::object self, const std::string& s, pybind11::object value);



class Component : public sofa::core::objectmodel::BaseComponent {
public:
    SOFA_CLASS(Component, sofa::core::objectmodel::BaseComponent);
    void init() override {}
    void reinit() override {}
};

class Component_Trampoline : public Component, public TrampolineBase
{
public:
    SOFA_CLASS(Component_Trampoline, Component);

    Component_Trampoline();
    ~Component_Trampoline() override = default;

    void init() override;
    void reinit() override;
    void draw(const sofa::core::visual::VisualParams* params) override;
    void handleEvent(sofa::core::objectmodel::Event* event) override;
    std::string getClassName() const override;
};

void moduleAddComponent(pybind11::module &m);

} /// namespace sofapython3
