#include "Binding_BaseObject.h"
#include "Binding_Controller.h"

#include "PythonDownCast.h"

namespace sofapython3
{
void moduleAddBaseObject(py::module& m)
{
    PythonDownCast::registerType<sofa::core::objectmodel::BaseObject>(
                [](sofa::core::objectmodel::Base* object)
    {
        return py::cast(object->toBaseObject());
    });

    py::class_<BaseObject, Base, BaseObject::SPtr>p(m, "BaseObject");
    p.def("init", &BaseObject::init);
    p.def("reinit", &BaseObject::init);

    p.def("getPathName", &BaseObject::getPathName);
    p.def("getLink", [](const BaseObject &self){ return std::string("@") + self.getPathName(); });
}
}
