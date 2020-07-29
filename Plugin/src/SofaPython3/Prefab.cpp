#include "Prefab.h"

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

namespace sofapython3
{
using sofa::core::objectmodel::Event;

void Prefab::init()
{
    std::cout << "prefab::init" << std::endl;
    reinit();
    Inherit1::init(sofa::core::ExecParams::defaultInstance());
}

void PrefabFileEventListener::fileHasChanged(const std::string &filename)
{
    PythonEnvironment::gil acquire ;
    //std::string file=filepath;
    //SP_CALL_FILEFUNC(const_cast<char*>("onReimpAFile"),
    //                 const_cast<char*>("s"),
    //                 const_cast<char*>(file.data()));

    py::dict local;
    local["filename"] = filename;
    py::eval("onReimpAFile(filename)", py::globals(), local);

    m_prefab->clearLoggedMessages();
    m_prefab->init();
}

void Prefab::reinit()
{
    std::cout << "prefab::reinit" << std::endl;
    clearLoggedMessages();

    /// remove everything in the node.
    execute<sofa::simulation::DeleteVisitor>(sofa::core::ExecParams::defaultInstance());
    std::cout << "prefab::doReInit" << std::endl;
    doReInit();

    std::cout << "simulation->initNode" << std::endl;
    /// Beurk beurk beurk
    sofa::simulation::getSimulation()->initNode(this);
    std::cout << "VisualInitVisitor" << std::endl;
    execute<VisualInitVisitor>(nullptr);

    m_componentstate = sofa::core::objectmodel::ComponentState::Valid;
}

void Prefab::doReInit()
{
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
        sofa::core::objectmodel::BaseData* data = sofapython3::addData(py::cast(this), name, py::none(), defaultValue, help, "Prefab's properties", type);
        m_datacallback.addInputs({data});
    }
}

void Prefab::setSourceTracking(const std::string& filename)
{
    std::cout << "Activating source tracking to " << filename << std::endl;
    FileMonitor::addFile(filename, &m_filelistener);
}

void Prefab::breakPrefab()
{
    std::cout << "Breaking prefab" << std::endl;
    FileMonitor::removeListener(&m_filelistener);
    for (auto& data : this->getDataFields())
        if (data->getGroup() == "Prefab's properties")
            m_datacallback.delInput(data);
}

}  // namespace sofapython3
