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

#include <pybind11/eval.h>
namespace py = pybind11;

#include <sofa/simulation/Node.h>
using sofa::simulation::Node;

#include <SofaSimulationGraph/DAGSimulation.h>
using sofa::simulation::graph::DAGSimulation ;
using sofa::simulation::Simulation;

#include <SofaSimulationGraph/SimpleApi.h>
namespace simpleapi = sofa::simpleapi;

#include <sofa/helper/Utils.h>
using sofa::helper::Utils;

#include <sofa/helper/system/FileRepository.h>
using sofa::helper::system::PluginRepository;

#include <sofa/simulation/SceneLoaderFactory.h>
using sofa::simulation::SceneLoaderFactory;
using sofa::simulation::SceneLoader;

#include <SofaPython3/SceneLoaderPY3.h>
using sofapython3::SceneLoaderPY3;

#include <sofa/helper/BackTrace.h>

#include <SofaSimulationCommon/init.h>
#include <SofaSimulationGraph/init.h>

#include <sofa/helper/system/FileRepository.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_Node.h>

#include "Timer/Submodule_Timer.h"

#include <SofaPython3/DataHelper.h>

#include <sofa/helper/logging/MessageDispatcher.h>
#include <sofa/helper/logging/ConsoleMessageHandler.h>
#include <sofa/core/logging/PerComponentLoggingMessageHandler.h>
using sofa::helper::logging::MessageDispatcher;
using sofa::helper::logging::MainPerComponentLoggingMessageHandler;
using sofa::helper::logging::MainConsoleMessageHandler;

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
        sofa::simulation::setSimulation(new DAGSimulation());
    }

    ~SofaInitializer(){
        sofa::simulation::common::cleanup();
        sofa::simulation::graph::cleanup();
    }
};



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
                   SofaRuntime.importPlugin("SofaComponentAll")

              )doc";

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

    m.def("init", []() {
        MessageDispatcher::clearHandlers();
        MessageDispatcher::addHandler(&MainConsoleMessageHandler::getInstance());
        MessageDispatcher::addHandler(&MainPerComponentLoggingMessageHandler::getInstance());
    });

    m.add_object("DataRepository", py::cast(&sofa::helper::system::DataRepository));
    m.add_object("PluginRepository", py::cast(&sofa::helper::system::PluginRepository));

    addSubmoduleTimer(m);
}

}  // namespace sofapython3
