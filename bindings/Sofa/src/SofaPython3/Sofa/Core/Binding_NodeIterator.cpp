/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2021 CNRS, University of Lille, INRIA                  *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

/// Neede to have automatic conversion from pybind types to stl container.
#include <pybind11/stl.h>
#include <SofaPython3/PythonFactory.h>
#include <SofaPython3/Sofa/Core/Binding_NodeIterator.h>

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

using sofa::core::objectmodel::BaseObject;

namespace sofapython3 {

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
        sofa::core::objectmodel::Base* obj = d.get_by_name(d.owner.get(), name);
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
        return PythonFactory::toPython(d.get(d.owner.get(), index).get());
    });
    d.def("remove_at", [](NodeIterator& d, size_t index)
    {
        return d.remove_at(d.owner.get(), index);
    });
    d.def("__contains__", [](NodeIterator& d, const std::string& name)
    {
        return d.get_by_name(d.owner.get(), name) != nullptr;
    });
}

} /// namespace sofapython3
