#include <pybind11/pybind11.h>

#include "Binding_GUIManager.h"

namespace py = pybind11;

namespace sofapython3 {
/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(SofaGui, m) {

    m.doc() = R"doc(
            SofaGui
            -----------------------

            Example of use:

                .. code-block:: python

                    from SofaGui import GUIManager
                    supported_gui = GUIManager.ListSupportedGUI()
                    print (supported_gui)

                .. automodule:: SofaGui
                    :toctree: _autosummary
                    :members:
             )doc";

    moduleAddGuiManager(m);
}
}