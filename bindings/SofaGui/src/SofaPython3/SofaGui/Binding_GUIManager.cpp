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

#include <pybind11/stl.h>

#include <SofaPython3/SofaGui/Binding_GUIManager.h>
#include <sofa/gui/common/GUIManager.h>
#include <sofa/gui/common/BaseGUI.h>
#include <sofa/simulation/Node.h>

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

namespace sofapython3 {

void moduleAddGuiManager(py::module& m)
{
    py::class_<sofa::gui::common::GUIManager> guiManager(m, "GUIManager", "Class managing the GUI creation, closing and its properties");

    /*
     * Sofa.Gui.GUIManager.ListSupportedGUI
     */
    const auto listSupportedGUIDoc = R"doc(
        List the supported GUIs.

        :param separator: Character used to separate the names of the GUIs. (optional)
        :type  separator: Char
        :return: A string containing each available GUIs. If the parameter 'separator' is omitted, a list containing the
                 available GUIs is returned instead of a string.
    )doc";
    guiManager.def_static("ListSupportedGUI", py::overload_cast<>(&sofa::gui::common::GUIManager::ListSupportedGUI), listSupportedGUIDoc);
    guiManager.def_static("ListSupportedGUI", py::overload_cast<char>(&sofa::gui::common::GUIManager::ListSupportedGUI), listSupportedGUIDoc );


    /*
     * Sofa.Gui.GUIManager.Init
     */
    const auto initDoc = R"doc(
        Initialize the GUI

        :param program_name: The name of the current program or script. (optional)
        :type  program_name: str
        :param gui_name: The name of the GUI used. See Sofa.Gui.GUIManager.ListSupportedGUI(). (optional)
        :type  gui_name: str
    )doc";
    guiManager.def_static("Init", &sofa::gui::common::GUIManager::Init, py::arg("program_name"), py::arg("gui_name") = "", initDoc);


    /*
     * Sofa.Gui.GUIManager.createGUI
     */
    const auto createGUIDoc = R"doc(
        Create the GUI.

        :param node: The root node of the current scene to draw into the GUI.
        :type node: Node&
        :param filename: The current script filename (optional)
        :type filename: str
        :return: 0 if the GUI creation succeed, 1 otherwise
    )doc";
    guiManager.def_static("createGUI", [](sofa::simulation::Node & node, const std::string & filename) -> int {
        return sofa::gui::common::GUIManager::createGUI(&node, filename.c_str());
    }, createGUIDoc);
    guiManager.def_static("createGUI", [](sofa::simulation::Node & node) -> int {
        return sofa::gui::common::GUIManager::createGUI(&node);
    }, createGUIDoc);


    /*
     * Sofa.Gui.GUIManager.closeGUI
     */
    const auto closeGUIDoc = R"doc(
        Close the current GUI.
    )doc";
    guiManager.def_static("closeGUI", &sofa::gui::common::GUIManager::closeGUI, closeGUIDoc);


    /*
     * Sofa.Gui.GUIManager.SetScene
     */
    const auto setSceneDoc = R"doc(
        Set the active scene.

        :param node: The root node of the current scene to draw into the GUI.
        :type node: Node&
        :param filename: The current script filename (optional)
        :type filename: str
        :param temporaryFile: Whether the file is temporary (default to false)
        :type temporaryFile: bool
    )doc";
    guiManager.def_static("SetScene", [](sofa::simulation::Node & node, const char * filename = nullptr, bool temporaryFile = false) {
        sofa::gui::common::GUIManager::SetScene(&node, filename, temporaryFile);
    }, py::arg("root"), py::arg("filename") = nullptr, py::arg("temporaryFile") = false, setSceneDoc);


    /*
     * Sofa.Gui.GUIManager.MainLoop
     */
    const auto mainLoopDoc = R"doc(
        Main loop of the GUI.

        Calling the main loop will steal the execution control of the script until the GUI is closed.

        :param node: The root node of the current scene to draw into the GUI.
        :type node: Node&
        :param filename: The current script filename (optional)
        :type filename: str
        :return: 0 if the main loop succeed, 1 otherwise
    )doc";
    guiManager.def_static("MainLoop", [](sofa::simulation::Node & node, const std::string & filename) -> int {
        return sofa::gui::common::GUIManager::MainLoop(&node, filename.c_str());
    }, mainLoopDoc);
    guiManager.def_static("MainLoop", [](sofa::simulation::Node & node) -> int {
        return sofa::gui::common::GUIManager::MainLoop(&node);
    }, mainLoopDoc);


    /*
     * Sofa.Gui.GUIManager.SetDimension
     */
    const auto setDimensionDoc = R"doc(
        Set the GUI's dimensions.

        This must be called after Sofa.Gui.GUIManager.createGUI

        :param width:
        :type width: int
        :param height:
        :type height: int
    )doc";
    guiManager.def_static("SetDimension", &sofa::gui::common::GUIManager::SetDimension, setDimensionDoc);

    /*
     * Sofa.Gui.GUIManager.CenterWindow
     */
    const auto centerWindowDoc = R"doc(
        Center the GUI's window on screen .

        This must be called after Sofa.Gui.GUIManager.createGUI

    )doc";
    guiManager.def_static("CenterWindow", &sofa::gui::common::GUIManager::CenterWindow, centerWindowDoc);

    /*
     * Sofa.Gui.GUIManager.SetFullScreen
     */
    const auto SetFullScreenDoc = R"doc(
        Set the GUI in full screen mode.
    )doc";
    guiManager.def_static("SetFullScreen", &sofa::gui::common::GUIManager::SetFullScreen, SetFullScreenDoc);

    /*
     * Sofa.Gui.GUIManager.SaveScreenshot
     */
    const auto SaveScreenshotDoc = R"doc(
        Save the current frame into a file.

        :param filename: Where to save the screenshot.
        :type filename: str
    )doc";
    guiManager.def_static("SaveScreenshot", &sofa::gui::common::GUIManager::SaveScreenshot, SaveScreenshotDoc);

    /*
     * Sofa.Gui.GUIManager.GetGUI
     */
    const auto GetGUIDoc = R"doc(
        Get the current GUI.
    )doc";
    guiManager.def_static("GetGUI", &sofa::gui::common::GUIManager::getGUI, GetGUIDoc);
}

} /// namespace sofapython3
