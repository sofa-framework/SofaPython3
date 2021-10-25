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

#include <pybind11/pybind11.h>

#include <SofaGui/initSofaGui.h>
#include <sofa/core/init.h>
#include <sofa/helper/logging/Messaging.h>
#include <sofa/helper/Utils.h>
#include <sofa/helper/system/FileSystem.h>
using sofa::helper::system::FileSystem;

#if SOFAGUI_HAVE_SOFAGUIQT
#include <sofa/gui/qt/qt.conf.h>
#endif // SOFAGUI_HAVE_SOFAGUIQT

#include "Binding_BaseGui.h"
#include "Binding_GUIManager.h"

namespace py = pybind11;

namespace sofapython3 {
/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(Gui, m) {

    m.doc() = R"doc(
            Sofa.Gui
            -----------------------

            Example of use:

                .. code-block:: python
                    import Sofa.Gui

                    supported_gui = Sofa.Gui.GUIManager.ListSupportedGUI(",")
                    print ("Supported GUIs are " + supported_gui)

                    Sofa.Gui.GUIManager.Init("gui_script_example")
                    Sofa.Gui.GUIManager.createGUI(root)
                    Sofa.Gui.GUIManager.MainLoop(root)
                    Sofa.Gui.GUIManager.closeGUI()


                .. automodule:: Gui
                    :toctree: _autosummary
                    :members:
             )doc";

#if SOFAGUI_HAVE_SOFAGUIQT
    std::string sofaPrefixAbsolute = sofa::helper::Utils::getSofaPathPrefix();
    std::string inputFilepath = FileSystem::cleanPath(sofaPrefixAbsolute + "/bin/qt.conf");
    bool success = sofa::gui::qt::loadQtConfWithCustomPrefix(inputFilepath, sofaPrefixAbsolute);
    if(success)
    {
        msg_info("Sofa.Gui") << "Loaded qt.conf from " << inputFilepath << " customized with Prefix = " << sofaPrefixAbsolute;
    }
    else
    {
        msg_warning("Sofa.Gui") << "Failed loading and/or customizing qt.conf from " << inputFilepath;

        std::cout << "qt_resource_data:" << std::endl;
        for (int i = 0 ; i < qt_resource_data.size() ; ++i) {
           std::cout << qt_resource_data[i];
        }
        std::cout << std::endl;
    }
#endif // SOFAGUI_HAVE_SOFAGUIQT

    // This is needed to make sure the GuiMain library (libSofaGuiMain.so) is correctly
    // linked since the GUIs are statically created during the load of the library.
    sofa::gui::initSofaGui();
    sofa::core::init();

    moduleAddBaseGui(m);
    moduleAddGuiManager(m);
}

} // namespace sofapython3
