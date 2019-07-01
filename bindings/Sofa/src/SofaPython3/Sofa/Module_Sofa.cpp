#include "Core/Submodule_Core.h"
#include "Helper/Submodule_Helper.h"
#include "Simulation/Submodule_Simulation.h"
#include "Types/Submodule_Types.h"

namespace sofapython3
{

/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(Sofa, m)
{
    m.doc() = R"doc(
              Sofa
              -----------------------

              Example of use:
                .. code-block:: python

                   import Sofa

                   n = Sofa.Core.Node("MyNode"")
                   n.addChild("Node2")
                   n.addObject("MechanicalObject", name="dofs")

                   Sofa.Simulation.init(root)
                   Sofa.Simulation.print(root)

              Submodules:
                .. autosummary::
                  :toctree: _autosummary

                  Sofa.Core
                  Sofa.Simulation
                  Sofa.Types
                  Sofa.Helper
             )doc";
    py::module core = addSubmoduleCore(m);
    py::module helper = addSubmoduleHelper(m);
    py::module simulation = addSubmoduleSimulation(m);
    py::module types = addSubmoduleTypes(m);
}

} ///namespace sofapython3
