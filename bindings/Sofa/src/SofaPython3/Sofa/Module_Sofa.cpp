#include "Core/Submodule_Core.h"
#include "Helper/Submodule_Helper.h"
#include "Simulation/Submodule_Simulation.h"
#include "Types/Submodule_Types.h"

namespace sofapython3
{

/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(Sofa, m)
{
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
}

} ///namespace sofapython3
