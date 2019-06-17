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

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_Node.h>
#include <SofaPython3/Sofa/Core/Binding_Simulation.h>

#include <sofa/core/ObjectFactory.h>
#include <sofa/core/CategoryLibrary.h>

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

    py::module timer = addSubmoduleTimer(m);



    // Add the plugin directory to PluginRepository
    const std::string& pluginDir = Utils::getExecutableDirectory();
    PluginRepository.addFirstPath(pluginDir);

    //if( !sofa::simulation::getSimulation() )
    //    sofa::simulation::setSimulation(new DAGSimulation());

    /// We need to import the project dependencies
    py::module::import("Sofa");

    /// Check if there is already a SceneLoaderFactory. In case not load it.
    if( !SceneLoaderFactory::getInstance()->getEntryFileExtension("py3") )
    {
        std::cout << "Registering loader for python3 files" << std::endl ;
        SceneLoaderFactory::getInstance()->addEntry(new SceneLoaderPY3());
        sofa::helper::BackTrace::autodump();
    }

    m.def("importPlugin", [](const std::string& name)
    {
        return simpleapi::importPlugin(name);
    }, "imports a plugin given its name");

    m.def("getPythonVersion", [](){
        return std::to_string(PY_MAJOR_VERSION) + "." + std::to_string(PY_MINOR_VERSION);
    }, "returns the version of python currently in use");

    m.def("getSofaPath", [](){
        return Utils::getSofaPathPrefix();
    }, "returns SOFA_ROOT if set & valid, else returns the path to the executable currently in use");



    // TODO: move to submodule Sofa.Simulation
    m.def("exportSceneGraph", [](Node* node, const std::string& filename){
        sofa::simulation::getSimulation()->exportGraph( node, filename.c_str());
    }, "saves the scene graph to filename, using node as the root node");

    m.def("getAvailableComponents", [](){
        std::vector<sofa::core::ObjectFactory::ClassEntry::SPtr> entries ;
        sofa::core::ObjectFactory::getInstance()->getAllEntries(entries) ;

        py::dict d;
        for (auto& item : entries){
            d[item->className.c_str()] = item->description.c_str();
        }
        return d;
    }, "returns the list of all component registered in SOFA's object factory");

    m.def("getCategories", [](std::string component = ""){

        py::list list;

        std::vector<std::string> categories;
        if (component.empty())
            categories = sofa::core::CategoryLibrary::getCategories();
        else {
            sofa::core::ObjectFactory* factory = sofa::core::ObjectFactory::getInstance();

            if (factory->hasCreator(component))
            {
                sofa::core::ObjectFactory::ClassEntry entry = factory->getEntry(component);
                sofa::core::ObjectFactory::CreatorMap::iterator it2 = entry.creatorMap.begin();

                if( it2 != entry.creatorMap.end())
                {
                    sofa::core::ObjectFactory::Creator::SPtr c = it2->second;
                    const sofa::core::objectmodel::BaseClass* objClass = c->getClass();
                    sofa::core::CategoryLibrary::getCategories(objClass,categories);
                }
            }

        }
        for (auto& item : categories)
            list.append(item);
        return list;
    }, py::arg("component"), "returns the list of all basic component categories in SOFA."
                             " Optionally, takes a component name, and return the associated categories");

}
