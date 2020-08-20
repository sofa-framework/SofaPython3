#include "Binding_DataLink.h"

#include <sofa/core/objectmodel/BaseData.h>
using sofa::core::objectmodel::BaseData;


#include <SofaPython3/DataHelper.h>
#include <SofaPython3/PrefabParameter/DataLink.h>
using sofa::core::objectmodel::PrefabLink;
#include <SofaPython3/PythonFactory.h>

namespace sofapython3
{

using sofa::core::objectmodel::DataLink;

py::str __str__(DataLink& self)
{
    std::stringstream s;
    s << "Sofa.Core.DataLink<name='" << self.getName()
      << "', value='" << self.getValueString()
      << "', address='"<< reinterpret_cast<void*>(&self) <<"'>";
    return s.str();
}

py::str __repr__(DataLink& self)
{
    return py::repr(convertToPython(&self));
}

py::object getTargetBase(DataLink& self)
{
    auto base = self.getValue().getTargetBase().get();
    if (base)
        return PythonFactory::toPython(base);
    return py::none();
}

py::str getTargetPath(DataLink& self)
{
    return self.getValue().getTargetPath();
}

void setTargetPath(DataLink& self, const std::string& targetPath)
{
    self.beginEdit()->setTargetPath(targetPath);
    self.endEdit();
}

void setTargetBase(DataLink& self, Base::SPtr targetBase)
{
    self.beginEdit()->setTargetBase(targetBase);
    self.endEdit();
}

py::object __getattr__(DataLink& self, const std::string& s)
{
    if (s == "value")
        return py::cast<PrefabLink>(self.getValue());

    auto dataAlias = self.findDataAlias(s);
    if (dataAlias)
        return PythonFactory::toPython(dataAlias);
    return py::object();
}


void moduleAddDataLink(py::module &m)
{
    py::class_<PrefabLink, std::unique_ptr<PrefabLink, py::nodelete>> p(m, "PrefabLink");
    p.def("__repr__", [](PrefabLink& self){
        return "@" + self.getTargetBase()->getPathName();
    });

    py::class_<DataLink, BaseData, std::unique_ptr<DataLink, py::nodelete>> d(m, "DataLink");

    PythonFactory::registerType("DataLink", [](BaseData* data) -> py::object {
        return py::cast(reinterpret_cast<DataLink*>(data));
    });
    PythonFactory::registerType("Link", [](){ return new sofa::core::objectmodel::DataLink(); });


    d.def("__repr__",&__repr__);
    d.def("__str__", &__str__);
    d.def("getTargetBase", &getTargetBase);
    d.def("getTargetPath", &getTargetPath);
    d.def("setTargetBase", &setTargetBase);
    d.def("setTargetPath", &setTargetPath);
    d.def("__getattr__", &__getattr__);
}

}  // namespace sofapython3
