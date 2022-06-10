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

#include <SofaPython3/Prefab.h>
#include <SofaPython3/DataHelper.h>
#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/PythonEnvironment.h>

#include <sofa/simulation/DeleteVisitor.h>
#include <sofa/helper/system/FileMonitor.h>
using sofa::helper::system::FileMonitor;
using sofa::helper::system::FileEventListener;

#include <sofa/simulation/VisualVisitor.h>
using sofa::simulation::VisualInitVisitor;

#include <sofa/simulation/Simulation.h>
using sofa::simulation::Simulation;

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3
{
using sofa::core::objectmodel::Event;

void Prefab::initPrefab()
{
    Inherit1::init(sofa::core::execparams::defaultInstance());
    reinit();
}

void PrefabFileEventListener::fileHasChanged(const std::string &filename)
{
    PythonEnvironment::gil acquire ;
    py::dict local;
    local["filename"] = filename;
    py::eval("onReimpAFile(filename)", py::globals(), local);

    m_prefab->clearLoggedMessages();
    m_prefab->reinit();
}

void Prefab::init()
{
}

void Prefab::onParameterChanged()
{
    init();
}

void Prefab::reinit()
{
    d_componentState.setValue(sofa::core::objectmodel::ComponentState::Valid);
    clearLoggedMessages();

    /// remove everything in the node.
    execute<sofa::simulation::DeleteVisitor>(sofa::core::execparams::defaultInstance());

    onParameterChanged();

    sofa::simulation::getSimulation()->initNode(this);
    execute<VisualInitVisitor>(sofa::core::execparams::defaultInstance());
}

Prefab::Prefab()
{
    m_filelistener.m_prefab = this;
    m_datacallback.addCallback( std::bind(&Prefab::reinit, this) );
}

Prefab::~Prefab()
{
    FileMonitor::removeListener(&m_filelistener);
}

void Prefab::addPrefabParameter(const std::string& name, const std::string& help, const std::string& type, py::object defaultValue)
{
    if(!findData(name) && !findLink(name))
    {
        sofa::core::objectmodel::BaseData* data = sofapython3::addData(py::cast(this), name, py::none(), defaultValue, help, "Prefab's parameters", type);
        data->setRequired(true);
        m_datacallback.addInputs({data});
    }
}

void Prefab::setSourceTracking(const std::string& filename)
{
    FileMonitor::addFile(filename, &m_filelistener);
}

void Prefab::breakPrefab()
{
    FileMonitor::removeListener(&m_filelistener);
    for (auto& data : this->getDataFields())
        if (data->getGroup() == "Prefab's properties")
            m_datacallback.delInput(data);
}

}  // namespace sofapython3
