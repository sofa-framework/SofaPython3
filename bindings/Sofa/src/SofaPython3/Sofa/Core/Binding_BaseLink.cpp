#include "Binding_BaseLink.h"
#include "Binding_BaseLink_doc.h"

namespace sofapython3
{

void moduleAddBaseLink(py::module& m)
{
    py::class_<BaseLink> link(m, "Link");
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
    link.def("setHelp", [](BaseLink* self, const std::string value){
        /// The copy of a string into a char * is necessary because char * are temporary when using pybind11.
        /// For more information, see : https://github.com/pybind/pybind11/issues/1168
        char * S = new char[value.length() + 1];
        std::strcpy(S,value.c_str());
        self->setHelp(S);
    },  sofapython3::doc::baseLink::setHelp);

    link.def("getOwnerData", &BaseLink::getOwnerData, sofapython3::doc::baseLink::getOwnerData);
    link.def("getOwnerBase", &BaseLink::getOwnerBase, sofapython3::doc::baseLink::getOwnerBase);

    link.def("getLinkedData", &BaseLink::getLinkedData, sofapython3::doc::baseLink::getLinkedData);
    link.def("getLinkedBase", &BaseLink::getLinkedBase, sofapython3::doc::baseLink::getLinkedBase);

    link.def("getLinkedPath", &BaseLink::getLinkedPath, sofapython3::doc::baseLink::getLinkedPath);
    link.def("read", &BaseLink::read, sofapython3::doc::baseLink::read);

}

} /// namespace sofapython3
