#include <pybind11/eval.h>
namespace py = pybind11;

#include <sofa/simulation/Node.h>
using sofa::simulation::Node;

#include <SofaSimulationGraph/DAGSimulation.h>
using sofa::simulation::graph::DAGSimulation ;

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

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Simulation/Binding_Node.h>

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

static SofaInitializer s;

/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(SofaRuntime, m) {
    // Add the plugin directory to PluginRepository
    const std::string& pluginDir = Utils::getExecutableDirectory();
    PluginRepository.addFirstPath(pluginDir);

    if( !sofa::simulation::getSimulation() )
        sofa::simulation::setSimulation(new DAGSimulation());

    /// We need to import the project dependencies
    py::module::import("Sofa");

    /// Check if there is already a SceneLoaderFactory. In case not load it.
    if( !SceneLoaderFactory::getInstance()->getEntryFileExtension("py3") )
    {
        std::cout << "Registering loader for python3 files" << std::endl ;
        SceneLoaderFactory::getInstance()->addEntry(new SceneLoaderPY3());

        sofa::helper::BackTrace::autodump();
    }

    m.def("getSimulation", []()
    {
        return sofa::simulation::getSimulation();
    });
    
    m.def("importPlugin", [](const std::string& name)
    {
        return simpleapi::importPlugin(name);
    });

    m.def("reinit", []()
    {
        /// set the Simulation, replacing the existing one (which is automatically deleted)
        if( !sofa::simulation::getSimulation() )
            sofa::simulation::setSimulation(new DAGSimulation());
    });

    m.def("load", [](const std::string& filename) -> py::object
    {
        /// set the Simulation, replacing the existing one (which is automatically deleted)
        if( !sofa::simulation::getSimulation() )
            sofa::simulation::setSimulation(new DAGSimulation());

        SceneLoader* loader=SceneLoaderFactory::getInstance()->getEntryFileName(filename);
        if(!loader)
        {
            return py::none();

        }

        Node::SPtr root = loader->load(filename.c_str());
        return py::cast(root);
    });

    m.def("dev_getANode", []() -> py::object
    {
              Node::SPtr n = Node::create("testNode");
              return py::cast(n);
          });
}
