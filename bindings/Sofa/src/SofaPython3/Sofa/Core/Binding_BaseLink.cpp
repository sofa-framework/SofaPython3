/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2021 INRIA, USTL, UJF, CNRS, MGH                     *
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

#include <sofa/core/objectmodel/BaseLink.h>
using sofa::core::objectmodel::BaseLink;

#include <sofa/core/objectmodel/BaseObject.h>
using  sofa::core::objectmodel::BaseObject;

#include <sofa/core/objectmodel/BaseNode.h>

#include <SofaPython3/Sofa/Core/Binding_Base.h>
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

namespace {
py::object __getattr__(py::object self, const std::string& s)
{
    py::object base = getLinkedBase(py::cast<BaseLink&>(self), 0);
    if(!base.is_none())
    {
        return BindingBase::__getattr__(base, s);
    }
    throw std::runtime_error("Unable to find attribute on an empty link.");
}

void __setattr__(py::object self, const std::string& s, py::object value)
{
    py::object base = getLinkedBase(py::cast<BaseLink&>(self), 0);
    if(!base.is_none())
    {
        BindingBase::__setattr__(base, s, value);
        return;
    }
    throw std::runtime_error("Unable to find and set an attribute on an empty link.");
}
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

    link.def("getOwnerBase", getOwnerBase, sofapython3::doc::baseLink::getOwnerBase);

    link.def("getLinkedBase", getLinkedBase, "index"_a = 0, sofapython3::doc::baseLink::getLinkedBase);
    link.def("setLinkedBase", &BaseLink::setLinkedBase, sofapython3::doc::baseLink::getLinkedBase);

    link.def("getLinkedPath", &BaseLink::getLinkedPath, "index"_a = 0, sofapython3::doc::baseLink::getLinkedPath);
    link.def("getPathName", getPathName, sofapython3::doc::baseLink::getLinkedPath);
    link.def("read", &BaseLink::read, sofapython3::doc::baseLink::read);

    link.def("__getattr__", &__getattr__);
    link.def("__setattr__", &__setattr__);
}

} /// namespace sofapython3
