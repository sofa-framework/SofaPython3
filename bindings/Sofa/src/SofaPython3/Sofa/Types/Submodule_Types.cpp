#include "Submodule_Types.h"
#include "Binding_BoundingBox.h"

namespace sofapython3
{
/// The first parameter must be named the same as the module file to load.
py::module addSubmoduleTypes(py::module& p)
{
    py::module types = p.def_submodule("Types");

    types.doc() = R"doc(
           Default types available in Sofa
           -----------------------

           .. autosummary::
               Sofa.Types.Vec3
               Sofa.Types.BoundingBox
               ...

       )doc";

    moduleAddBoundingBox(types);

    return types;
}

}  // namespace sofapython3
