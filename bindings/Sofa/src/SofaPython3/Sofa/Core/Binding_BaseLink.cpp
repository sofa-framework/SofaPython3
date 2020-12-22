/*********************************************************************
Copyright 2019, CNRS, University of Lille, INRIA

This file is part of sofaPython3

sofaPython3 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

sofaPython3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
    - bruno.josue.marques@inria.fr
    - eve.le-guillou@centrale.centralelille.fr
    - jean-nicolas.brunet@inria.fr
    - thierry.gaugry@inria.fr
********************************************************************/

#include <sofa/core/objectmodel/BaseLink.h>
using sofa::core::objectmodel::BaseLink;

#include <sofa/core/objectmodel/BaseObject.h>
using  sofa::core::objectmodel::BaseObject;

#include <sofa/core/objectmodel/BaseNode.h>

#include <SofaPython3/Sofa/Core/Binding_BaseLink.h>
#include <SofaPython3/Sofa/Core/Binding_BaseLink_doc.h>
#include <SofaPython3/PythonFactory.h>

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

// To bring in the `_a` literal
using namespace pybind11::literals;

namespace sofapython3
{

void setHelp(BaseLink* self, const std::string value)
{
    /// The copy of a string into a char * is necessary because char * are temporary when using pybind11.
    /// For more information, see : https://github.com/pybind/pybind11/issues/1168
    char * S = new char[value.length() + 1];
    std::strcpy(S,value.c_str());
    self->setHelp(S);
}

py::object getLinkedBase(BaseLink& self, unsigned index = 0)
{
    if (self.getLinkedBase(index))
        return PythonFactory::toPython(self.getLinkedBase(index));
    return py::none();
}

py::object getOwnerBase(BaseLink& self)
{
    return PythonFactory::toPython(self.getOwnerBase());
}

std::string getPathName(BaseLink& self)
{
    auto n = self.getOwnerBase()->toBaseNode();
    auto o = self.getOwnerBase()->toBaseObject();
    return (n ? n->getPathName() : o->getPathName()) + "." + self.getName();
}

void moduleAddBaseLink(py::module& m)
{
    py::class_<BaseLink> link(m, "Link", sofapython3::doc::baseLink::baseLinkClass);
    link.def("getName", &BaseLink::getName, sofapython3::doc::baseLink::getName);
    link.def("setName", &BaseLink::setName, sofapython3::doc::baseLink::setName);
    link.def("isMultiLink", &BaseLink::isMultiLink, sofapython3::doc::baseLink::isMultiLink);

    link.def("isPersistent", &BaseLink::isPersistent, sofapython3::doc::baseLink::isPersistent);
    link.def("setPersistent", &BaseLink::setPersistent, sofapython3::doc::baseLink::setPersistent);

    link.def("isReadOnly", &BaseLink::isReadOnly, sofapython3::doc::baseLink::isReadOnly);
    link.def("getSize", &BaseLink::getSize, sofapython3::doc::baseLink::getSize);
    link.def("getValueString", &BaseLink::getValueString, sofapython3::doc::baseLink::getValueString);
    link.def("getValueTypeString", &BaseLink::getValueTypeString,  sofapython3::doc::baseLink::getValueTypeString);
    link.def("getHelp", &BaseLink::getHelp,  sofapython3::doc::baseLink::getHelp);
    link.def("setHelp", setHelp, sofapython3::doc::baseLink::setHelp);

    link.def("getOwnerData", &BaseLink::getOwnerData, sofapython3::doc::baseLink::getOwnerData);
    link.def("getOwnerBase", getOwnerBase, sofapython3::doc::baseLink::getOwnerBase);

    link.def("getLinkedData", &BaseLink::getLinkedData, sofapython3::doc::baseLink::getLinkedData);
    link.def("getLinkedBase", getLinkedBase, "index"_a = 0, sofapython3::doc::baseLink::getLinkedBase);
    link.def("setLinkedBase", &BaseLink::setLinkedBase, sofapython3::doc::baseLink::getLinkedBase);


    link.def("getLinkedPath", &BaseLink::getLinkedPath, "index"_a = 0, sofapython3::doc::baseLink::getLinkedPath);
    link.def("getPathName", getPathName, sofapython3::doc::baseLink::getLinkedPath);
    link.def("read", &BaseLink::read, sofapython3::doc::baseLink::read);

}

} /// namespace sofapython3
