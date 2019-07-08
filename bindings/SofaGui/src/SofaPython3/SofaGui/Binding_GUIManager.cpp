#include <pybind11/stl.h>

#include "Binding_GUIManager.h"
#include <sofa/gui/GUIManager.h>

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<std::string>);

namespace sofapython3 {

void moduleAddGuiManager(py::module& m)
{
    py::class_<sofa::gui::GUIManager> guiManager(m, "GUIManager");


    guiManager.def_static("ListSupportedGUI", py::overload_cast<>(&sofa::gui::GUIManager::ListSupportedGUI) );
    guiManager.def_static("ListSupportedGUI", py::overload_cast<char>(&sofa::gui::GUIManager::ListSupportedGUI) );
    guiManager.def_static("Init", &sofa::gui::GUIManager::Init);
    guiManager.def_static("Init", [](const std::string & program_name) {
        sofa::gui::GUIManager::Init(program_name.c_str());
    });

    guiManager.def_static("createGUI", [](sofa::simulation::Node & node, const std::string & filename) -> int {
        return sofa::gui::GUIManager::createGUI(&node, filename.c_str());
    });
    guiManager.def_static("createGUI", [](sofa::simulation::Node & node) -> int {
        return sofa::gui::GUIManager::createGUI(&node);
    });

    guiManager.def_static("closeGUI", &sofa::gui::GUIManager::closeGUI);

    guiManager.def_static("SetScene", [](sofa::simulation::Node & node, const std::string & filename, bool temporaryFile) {
        sofa::gui::GUIManager::SetScene(&node, filename.c_str(), temporaryFile);
    });
    guiManager.def_static("SetScene", [](sofa::simulation::Node & node, const std::string & filename) {
        sofa::gui::GUIManager::SetScene(&node, filename.c_str());
    });
    guiManager.def_static("SetScene", [](sofa::simulation::Node & node) {
        sofa::gui::GUIManager::SetScene(&node);
    });

    guiManager.def_static("MainLoop", [](sofa::simulation::Node & node, const std::string & filename) -> int {
        return sofa::gui::GUIManager::MainLoop(&node, filename.c_str());
    });
    guiManager.def_static("MainLoop", [](sofa::simulation::Node & node) -> int {
        return sofa::gui::GUIManager::MainLoop(&node);
    });
}

} /// namespace sofapython3