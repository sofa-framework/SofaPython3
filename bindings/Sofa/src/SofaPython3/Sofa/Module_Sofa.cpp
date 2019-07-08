#include "Core/Submodule_Core.h"
#include "Helper/Submodule_Helper.h"
#include "Simulation/Submodule_Simulation.h"
#include "Types/Submodule_Types.h"

namespace sofapython3
{

/// The first parameter must be named the same as the module file to load.
PYBIND11_MODULE(Core, c)
{
    addSubmoduleCore(c);
}
PYBIND11_MODULE(Helper, h)
{
    addSubmoduleHelper(h);
}
PYBIND11_MODULE(Simulation, s)
{
    addSubmoduleSimulation(s);
}
PYBIND11_MODULE(Types, t)
{
    addSubmoduleTypes(t);
}

} ///namespace sofapython3
