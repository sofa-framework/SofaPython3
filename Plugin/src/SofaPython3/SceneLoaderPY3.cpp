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

#include <sstream>
#include <fstream>

#include <sofa/simulation/graph/DAGNode.h>
using sofa::simulation::graph::DAGNode;

#include <SofaPython3/PythonEnvironment.h>
#include <SofaPython3/SceneLoaderPY3.h>
#include <SofaPython3/PythonFactory.h>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
namespace py = pybind11;

#include <sofa/helper/system/SetDirectory.h>
using sofa::helper::system::SetDirectory;

using namespace sofa::core::objectmodel;

MSG_REGISTER_CLASS(sofapython3::SceneLoaderPY3, "SofaPython3::SceneLoader")

PYBIND11_DECLARE_HOLDER_TYPE(Base, sofa::core::sptr<Base>, true)
template class py::class_<sofa::core::objectmodel::Base,
sofa::core::sptr<sofa::core::objectmodel::Base>>;

namespace sofapython3
{

bool SceneLoaderPY3::canLoadFileExtension(const char *extension)
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext=="py" || ext=="py3" || ext=="py3scn" || ext=="pyscn");
}

bool SceneLoaderPY3::canWriteFileExtension(const char *extension)
{
    return canLoadFileExtension(extension);
}

/// get the file type description
std::string SceneLoaderPY3::getFileTypeDesc()
{
    return "Python3 Scenes";
}

/// get the list of file extensions
void SceneLoaderPY3::getExtensionList(ExtensionList* list)
{
    list->clear();
    list->push_back("py3scn");
    list->push_back("py3");
    list->push_back("pyscn");
    list->push_back("py");
}

bool SceneLoaderPY3::syntaxForAddingRequiredPlugin(const std::string& pluginName,
                                                   const std::vector<std::string>& listComponents, std::ostream& ss, sofa::simulation::Node* nodeWhereAdded)
{
    ss << nodeWhereAdded->getName() << ".addObject('RequiredPlugin', name='" << pluginName << "')";
    if (!listComponents.empty())
    {
        ss <<  " # Needed to use components [" << sofa::helper::join(listComponents, ',');
    }
    ss << "]" << msgendl;
    return true;
}

sofa::simulation::Node::SPtr SceneLoaderPY3::doLoad(const std::string& filename, const std::vector<std::string>& sceneArgs)
{
    sofa::simulation::Node::SPtr root = sofa::simulation::Node::create("root");
    loadSceneWithArguments(filename.c_str(), sceneArgs, root);
    return root;
}


void SceneLoaderPY3::loadSceneWithArguments(const char *filename,
                                            const std::vector<std::string>& arguments,
                                            Node::SPtr root_out)
{
    PythonEnvironment::setArguments(SetDirectory::GetFileName(filename).c_str(), arguments);
    PythonEnvironment::gil lock;

    try{
        py::module::import("Sofa.Core");
        py::object globals = py::module::import("__main__").attr("__dict__");
        py::module module;

        SetDirectory localDir(filename);
        std::string basename = SetDirectory::GetFileNameWithoutExtension(SetDirectory::GetFileName(filename).c_str());
        module = PythonEnvironment::importFromFile(basename, SetDirectory::GetFileName(filename), &globals);

        if(!py::hasattr(module, "createScene"))
        {
            msg_error("SofaPython3") << "Missing createScene function";
            return ;
        }

        py::object createScene = module.attr("createScene");
        createScene( PythonFactory::toPython(root_out.get()) );

        root_out->setInstanciationSourceFileName(filename);
        root_out->setInstanciationSourceFilePos(0);
    }catch(py::error_already_set& e)
    {
        msg_error() << "Unable to completely load the scene from file '"<< filename << "'." << msgendl
                    << "Python exception: " << msgendl
                    << "  " << e.what();
    }

}

} // namespace sofapython3

