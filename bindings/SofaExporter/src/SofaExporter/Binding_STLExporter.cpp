#include <SofaExporter/Binding_STLExporter.h>
#include <SofaExporter/Binding_STLExporter_doc.h>

#include <SofaPython3/PythonDownCast.h>

using  sofa::component::exporter::STLExporter;

namespace sofapython3
{

void moduleAddSTLExporter(py::module &m)
{
    PythonDownCast::registerType<STLExporter>(
                [](sofa::core::objectmodel::Base* object)
    {
        return py::cast(dynamic_cast<STLExporter*>(object));
    });

    py::class_<STLExporter, sofa::core::objectmodel::BaseObject, sofa::core::sptr<STLExporter>> p(m, "STLExporter");

    p.def("write", &STLExporter::write, sofapython3::doc::SofaExporter::STLExporter::write::docstring);
}

} // namespace sofapython3
