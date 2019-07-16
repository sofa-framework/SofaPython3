#include "Submodule_System.h"
#include "Binding_FileRepository.h"

namespace sofapython3
{

void moduleAddSystem(py::module &m)
{
    auto system = m.def_submodule("System");

    moduleAddFileRepository(system);
}

} ///namespace sofapython3