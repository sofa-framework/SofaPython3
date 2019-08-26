/// Neede to have automatic conversion from pybind types to stl container.
#include <pybind11/stl.h>
#include <SofaPython3/PythonFactory.h>
#include "Binding_NodeIterator.h"

namespace sofapython3
{

void moduleAddNodeIterator(py::module &m)
{
    py::class_<NodeIterator> d(m, "NodeIterator");

    d.def("__getitem__", [](NodeIterator& d, size_t index) -> py::object
    {
        if(index>=d.size(d.owner.get()))
            throw py::index_error("Too large index '"+std::to_string(index)+"'");
        return PythonFactory::toPython(d.get(d.owner.get(), index).get());
    });

    d.def("__getitem__", [](NodeIterator& d, const std::string& name) -> py::object
    {
        BaseObject* obj =d.owner->getObject(name);
        if(obj==nullptr)
            throw py::index_error("No existing object '"+name+"'");
        return PythonFactory::toPython(obj);
    });

    d.def("__iter__", [](NodeIterator& d)
    {
        return d;
    });

    d.def("__next__", [](NodeIterator& d) -> py::object
    {
        if(d.index>=d.size(d.owner.get()))
            throw py::stop_iteration();
        return PythonFactory::toPython(d.get(d.owner.get(), d.index++).get());
    });
    d.def("__len__", [](NodeIterator& d) -> py::object
    {
        return py::cast(d.size(d.owner.get()));
    });

    d.def("at", [](NodeIterator& d, size_t index) -> py::object
    {
        return py::cast(d.get(d.owner.get(), index));
    });
    d.def("remove_at", [](NodeIterator& d, size_t index)
    {
        BaseNode::SPtr n(dynamic_cast<BaseNode*>(d.get(d.owner.get(), index).get()));
        d.owner->removeChild(n);
    });
}

} /// namespace sofapython3
