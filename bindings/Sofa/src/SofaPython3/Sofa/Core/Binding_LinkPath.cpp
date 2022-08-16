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

#include <SofaPython3/Sofa/Core/Binding_LinkPath.h>
#include <SofaPython3/Sofa/Core/Binding_LinkPath_doc.h>

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

// To bring in the `_a` literal
using namespace pybind11::literals;

namespace sofapython3
{

LinkPath::LinkPath(sofa::core::sptr<sofa::core::objectmodel::Base> target)
{
    targetBase = target;
    targetData = nullptr;
}

LinkPath::LinkPath(sofa::core::objectmodel::BaseData* target)
{
    // If the data is attached to
    if(target->getOwner())
    {
        targetBase = target->getOwner();
    }
    targetData = target;
}

bool LinkPath::isPointingToData() const
{
    return targetData != nullptr;
}

std::string __str__(const LinkPath& entry)
{
    std::ostringstream s;
    if(entry.targetData != nullptr)
        return entry.targetData->getLinkPath();
    else if(entry.targetBase.get() != nullptr)
        return "@" + entry.targetBase->getPathName();
    throw std::runtime_error("Empty LinkPath");
}

std::string __repr__(const LinkPath& entry)
{
    std::ostringstream s;
    s << "LinkPath(\"" << __str__(entry) << "\")";
    return s.str();
}

void moduleAddLinkPath(py::module& m)
{
    py::class_<LinkPath> link(m, "LinkPath", sofapython3::doc::linkpath::linkpath);
    link.def("__str__", &__str__);
    link.def("__repr__", &__repr__);
}

}/// namespace sofapython3
