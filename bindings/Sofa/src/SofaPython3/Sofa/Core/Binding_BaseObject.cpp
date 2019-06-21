#include "Binding_BaseObject.h"
#include "Binding_Controller.h"

#include "PythonDownCast.h"

namespace sofapython3
{
py::object getItem(const BaseObject& self, const std::string& path)
{
    if (path.empty())
        return py::cast(self);

    BaseData* data = self.findData(path);
    if (data)
        return py::cast(data);
    sofa::core::objectmodel::BaseLink* link = self.findLink(path);
    if (link)
        return py::cast(link);
    throw py::value_error("Invalid syntax"); // should never get there
}

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

    /// gets an item using its path (path is dot-separated, relative to the object
    /// it's called upon & ONLY DESCENDING (no ../):
    ///
    /// This method lifts most ambiguities when accessing a node, object or data
    /// from a path relative to self.
    ///
    /// examples:
    /// ------------
    ///
    /// root["node1.node2.object1.value"]
    ///
    /// In the example above, node1 and node2 can be inferred as being nodes without performing any checks.
    /// object1 can be a node or an object, but cannot be a datafield nor a link
    /// value can be a node or an object (if object1 is a node), or must be a data (if object1 is an object)
    p.def("__getitem__", [](BaseObject &self, std::string s) -> py::object
    {
        if (s[0] == '.')
            s.erase(s.begin());
        std::list<std::string> stringlist;
        std::istringstream iss(s);
        std::string token;
        while (std::getline(iss, token, '.'))
        {
            if (!token.empty())
                stringlist.push_back(token);
        }

        // perform here the syntax checks over the string to parse

        if (stringlist.empty())
            return py::cast(self);
        if (stringlist.size() > 1)
            throw py::value_error("Invalid syntax");

        return getItem(self, s);
    });

}
}  // namespace sofapython3
