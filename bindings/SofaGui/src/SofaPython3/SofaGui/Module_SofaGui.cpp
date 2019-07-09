#include <pybind11/pybind11.h>

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

    moduleAddGuiManager(m);
}
}