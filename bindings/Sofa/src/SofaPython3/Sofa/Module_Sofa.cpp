#include "Core/Submodule_Core.h"
#include "Helper/Submodule_Helper.h"
#include "Simulation/Submodule_Simulation.h"
#include "Types/Submodule_Types.h"

namespace sofapython3
{

/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(Sofa, m)
{
    //py::options options;
    //options.disable_function_signatures();

    m.doc() = R"doc(
           Sofa binding
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

           Functions:
              .. autosummary::

                Sofa.msg_info
                Sofa.msg_warning
                Sofa.msg_error
                Sofa.msg_fatal
                Sofa.msg_deprecated
       )doc";

    py::module core = addSubmoduleCore(m);
    py::module helper = addSubmoduleHelper(m);
    py::module simulation = addSubmoduleSimulation(m);
    py::module types = addSubmoduleTypes(m);

    /// Import into the Sofa main package the class from theyr sub-module if they can be instantiated
    m.add_object("Controller", core.attr("Controller"));
    m.add_object("ForceField", core.attr("ForceField"));
    m.add_object("Node", core.attr("Node"));

    m.add_object("msg_info", helper.attr("msg_info"));
    m.add_object("msg_warning", helper.attr("msg_warning"));
    m.add_object("msg_error", helper.attr("msg_error"));
    m.add_object("msg_fatal", helper.attr("msg_fatal"));
    m.add_object("msg_deprecated", helper.attr("msg_deprecated"));

    /// Import some binding part fully written in python (because I'm too lazy)
    py::module st = m.def_submodule("Types", R"doc(
                                             Data types
                                             -----------------------

                                             .. autosummary::
                                                 Sofa.Types.RGBAColor
                                                 Sofa.Types.Vec3
                                             )doc");
    py::module stimpl = py::module::import("__Sofa_Types__");
    st.add_object("RGBAColor", stimpl.attr("RGBAColor"));
    st.add_object("Vec3", stimpl.attr("Vec3"));
}

} ///namespace sofapython3
