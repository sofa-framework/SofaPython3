#include "Binding_DataLink.h"

#include <sofa/core/objectmodel/BaseData.h>
using sofa::core::objectmodel::BaseData;


#include <SofaPython3/DataHelper.h>
using sofa::core::objectmodel::PrefabLink;
#include <SofaPython3/PythonFactory.h>

namespace sofapython3
{

py::str getTargetPath(const PrefabLink& link)
{
    return link.getTargetPath();
}

py::object getTargetBase(const PrefabLink& link)
{
    auto base = link.getTargetBase().get();
    if (base)
        return PythonFactory::toPython(base);
    return py::none();
}

py::str DataLink::__str__()
{
    std::stringstream s;
    s << "Sofa.Core.DataLink<name='" << getName()
      << "', value='" << getValueString()
      << "', address='"<< (void*)this <<"'>";
    return s.str();
}

py::str DataLink::__repr__()
{
    return py::repr(convertToPython(this));
}

py::str DataLink::getTargetPath()
{
    return sofapython3::getTargetPath(reinterpret_cast<sofa::core::objectmodel::DataLink*>(this)->getValue());
}

py::object DataLink::getTargetBase()
{
    return sofapython3::getTargetBase(reinterpret_cast<sofa::core::objectmodel::DataLink*>(this)->getValue());
}


void moduleAddDataLink(py::module &m)
{
    py::class_<PrefabLink, std::unique_ptr<PrefabLink, py::nodelete>> l(m, "PrefabLink");
    l.def(py::init<const Base::SPtr&>());
    l.def(py::init<BaseLink*>());
    l.def(py::init<const std::string&>());
    l.def("getTargetBase", &getTargetBase);
    l.def("getTargetPath", &getTargetPath);

    py::class_<DataLink, BaseData, std::unique_ptr<DataLink, py::nodelete>> d(m, "DataLink");

    PythonFactory::registerType("PrefabLink", [](BaseData* data) -> py::object {
        return py::cast(reinterpret_cast<DataLink*>(data));
    });

    d.def("__repr__",&DataLink::__repr__);
    d.def("__str__", &DataLink::__str__);
    d.def("getTargetBase", &DataLink::getTargetBase);
    d.def("getTargetPath", &DataLink::getTargetPath);
}

}  // namespace sofapython3
