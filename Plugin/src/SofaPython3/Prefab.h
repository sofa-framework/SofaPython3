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
#include <sofa/simulation/Node.h>
#include <sofa/core/objectmodel/DataCallback.h>

#include <sofa/helper/system/FileMonitor.h>
#include <SofaPython3/PythonEnvironment.h>

namespace sofa::core::objectmodel
{

class SOFAPYTHON3_API BasePrefab : public sofa::simulation::Node
{
public:
    SOFA_CLASS(BasePrefab, sofa::simulation::Node);
};

}  // namespace sofa::core::objectmodel

namespace sofapython3
{
using sofa::simulation::Node;
using sofa::core::objectmodel::BasePrefab;
using sofa::core::objectmodel::DataCallback;

class Prefab;
class SOFAPYTHON3_API PrefabFileEventListener : public sofa::helper::system::FileEventListener
{
public:
    Prefab* m_prefab;

    void fileHasChanged(const std::string& filename) override;
};

class SOFAPYTHON3_API Prefab : public BasePrefab
{
public:
    SOFA_CLASS(Prefab, BasePrefab);
    void init();
    void reinit();
    virtual void doReInit() ;

    static const std::string GetCustomClassName(){ return "Prefab"; }

    void addPrefabParameter(const std::string& name, const std::string& help, const std::string& type, pybind11::object defaultValue = pybind11::none());
    void setSourceTracking(const std::string& filename);
    void breakPrefab();

    Prefab();
    ~Prefab() override;

    PrefabFileEventListener m_filelistener;
    DataCallback m_datacallback;
    bool m_is_initialized {false};
};
}  // namespace sofapython3
