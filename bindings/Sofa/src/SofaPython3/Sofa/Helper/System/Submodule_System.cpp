#include "Submodule_System.h"
#include "Binding_FileRepository.h"

#include <sofa/helper/system/FileRepository.h>

namespace sofapython3
{

void moduleAddSystem(py::module &m)
{
    auto system = m.def_submodule("System");

    moduleAddFileRepository(system);

    m.add_object("DataRepository", py::cast(&sofa::helper::system::DataRepository));
    m.add_object("PluginRepository", py::cast(&sofa::helper::system::PluginRepository));
}

} ///namespace sofapython3