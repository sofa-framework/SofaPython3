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

#include <pybind11/eval.h>
#include <sofa/helper/system/PluginManager.h>
namespace py = pybind11;

#include <sofa/simulation/graph/DAGSimulation.h>
using sofa::simulation::graph::DAGSimulation ;
using sofa::simulation::Simulation;

#include <sofa/simpleapi/SimpleApi.h>
namespace simpleapi = sofa::simpleapi;

#include <sofa/helper/Utils.h>
using sofa::helper::Utils;

#include <sofa/helper/system/FileRepository.h>
using sofa::helper::system::PluginRepository;

#include <sofa/simulation/SceneLoaderFactory.h>
using sofa::simulation::SceneLoaderFactory;
using sofa::simulation::SceneLoader;


#include <pybind11/stl.h>
#include <sofa/core/ObjectFactory.h>
using sofa::core::ObjectFactory;
#include <sofa/core/CategoryLibrary.h>
using sofa::core::CategoryLibrary;



#include <SofaPython3/SceneLoaderPY3.h>
using sofapython3::SceneLoaderPY3;

#include <sofa/helper/BackTrace.h>

#include <sofa/simulation/common/init.h>
#include <sofa/simulation/graph/init.h>

#include "Timer/Submodule_Timer.h"

#include <sofa/helper/logging/MessageDispatcher.h>
#include <sofa/helper/logging/ConsoleMessageHandler.h>
#include <sofa/core/logging/PerComponentLoggingMessageHandler.h>
using sofa::helper::logging::MessageDispatcher;
using sofa::helper::logging::MainPerComponentLoggingMessageHandler;
using sofa::helper::logging::MainConsoleMessageHandler;

#include <sofa/core/init.h>
#include <sofa/helper/init.h>
#include <sofa/simulation/init.h>

namespace sofapython3
{


class SofaInitializer
{
public:
    // TODO, ces trucs sont fort laid. Normalement ce devrait être une joli plugin qui
    // appelle le init.
    SofaInitializer(){
        sofa::simulation::common::init();
        sofa::simulation::graph::init();
    }

    ~SofaInitializer(){
        sofa::simulation::common::cleanup();
        sofa::simulation::graph::cleanup();
    }
};

static std::vector<std::string>  getCategories(const std::string& className)
{
    std::vector<std::string> categories;
    ObjectFactory* factory = ObjectFactory::getInstance();

    if (factory->hasCreator(className))
    {
        ObjectFactory::ClassEntry& entry = factory->getEntry(className);
        ObjectFactory::CreatorMap::iterator it = entry.creatorMap.begin();

        if( it != entry.creatorMap.end())
        {
            ObjectFactory::Creator::SPtr c = it->second;
            const sofa::core::objectmodel::BaseClass* objClass = c->getClass();
            CategoryLibrary::getCategories(objClass,categories);
        }
    }
    return categories ;
}

static SofaInitializer s;

/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(SofaRuntime, m) {

    m.doc() = R"doc(
              Expose aspect specific to the application/runtime
              -------------------------------------------------

              .. autosummary::
                  :toctree:_autosummary/_autosummary

                  SofaRuntime.importPlugin


              Example of use:
                .. code-block:: python

                   import SofaRuntime
                   SofaRuntime.importPlugin("Sofa.Component.LinearSolver")

              )doc";

    // These are needed to force the dynamic loading of module dependencies (found in CMakeLists.txt)
    sofa::core::init();
    sofa::helper::init();
    sofa::simulation::core::init();
    sofa::simulation::graph::init();
    sofa::simulation::common::init();

    // Add the plugin directory to PluginRepository
    const std::string& pluginDir = Utils::getExecutableDirectory();
    PluginRepository.addFirstPath(pluginDir);

    /// We need to import the project dependencies
    py::module::import("Sofa.Core");
    py::module::import("Sofa.Helper");

    /// Check if there is already a SceneLoaderFactory. In case not load it.
    if( !SceneLoaderFactory::getInstance()->getEntryFileExtension("py3") )
    {
        SceneLoaderFactory::getInstance()->addEntry(new SceneLoaderPY3());
        sofa::helper::BackTrace::autodump();
    }

    m.def("importPlugin", [](const std::string& name)
    {
        return simpleapi::importPlugin(name);
    }, "import a sofa plugin into the current environment");

    m.def("unload_plugin", [](const std::string& name)
    {
        auto& pluginManager = sofa::helper::system::PluginManager::getInstance();
        pluginManager.unloadPlugin(name);
    }, "unload a sofa plugin");

    m.def("load_plugins_from_ini_file", [](const std::string& iniFile)
    {
        auto& pluginManager = sofa::helper::system::PluginManager::getInstance();
        pluginManager.readFromIniFile(iniFile);
    }, "import a list of plugins defined in a file");

    m.def("auto_load_plugins", []()
    {
        std::string configPluginPath = "plugin_list.conf";
        std::string defaultConfigPluginPath = "plugin_list.conf.default";
        if (sofa::helper::system::PluginRepository.findFile(configPluginPath, "", nullptr))
        {
            msg_info("SofaRuntime") << "Loading automatically plugin list in " << configPluginPath;
            sofa::helper::system::PluginManager::getInstance().readFromIniFile(configPluginPath);
        }
        if (sofa::helper::system::PluginRepository.findFile(defaultConfigPluginPath, "", nullptr))
        {
            msg_info("SofaRuntime") << "Loading automatically plugin list in " << defaultConfigPluginPath;
            sofa::helper::system::PluginManager::getInstance().readFromIniFile(defaultConfigPluginPath);
        }
    }, "automatically load plugins from configuration files");

    m.def("init", []() {
        MessageDispatcher::clearHandlers();
        MessageDispatcher::addHandler(&MainConsoleMessageHandler::getInstance());
        MessageDispatcher::addHandler(&MainPerComponentLoggingMessageHandler::getInstance());
    });

    m.add_object("DataRepository", py::cast(&sofa::helper::system::DataRepository));
    m.add_object("PluginRepository", py::cast(&sofa::helper::system::PluginRepository));
    m.def("getCategories", &getCategories);

    addSubmoduleTimer(m);
}

}  // namespace sofapython3
