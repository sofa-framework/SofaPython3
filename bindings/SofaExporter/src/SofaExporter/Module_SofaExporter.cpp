#include <pybind11/eval.h>
namespace py = pybind11;

#include <SofaPython3/Sofa/Core/Binding_Base.h>
#include <SofaPython3/Sofa/Core/Binding_BaseObject.h>

#include <SofaExporter/Binding_STLExporter.h>

namespace sofapython3
{

PYBIND11_MODULE(SofaExporter, m) {
    m.doc() = R"doc(
              SofaRuntime
              -----------------------

              Provides python bindings for the SofaExporter module

              Example of use:
              .. code-block:: python

              import SofaExporter

              .. automethod::
              :toctree: _autosummary

              )doc";

    py::module::import("Sofa.Core");

    moduleAddSTLExporter(m);
}

}  // namespace sofapython3

