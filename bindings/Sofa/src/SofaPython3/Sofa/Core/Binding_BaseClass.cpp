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

#include <SofaPython3/Sofa/Core/Binding_BaseClass.h>
#include <SofaPython3/Sofa/Core/Binding_BaseClass_doc.h>
#include <sofa/core/objectmodel/BaseClass.h>

namespace sofapython3
{
using namespace sofa::core::objectmodel;

/// Makes an alias for the pybind11 namespace to increase readability.
namespace py { using namespace pybind11; }

auto getBaseClassBinding(py::module& m)
{
    static py::class_<BaseClass, std::unique_ptr<BaseClass, py::nodelete>> base(m, "BaseClass", doc::baseclass::classdocstring);
    return base;
}

void moduleForwardAddBaseClass(py::module& m)
{
    getBaseClassBinding(m);
}

void moduleAddBaseClass(py::module &m)
{
    // adds here the BaseClass's binded function (if any).
}

} /// namespace sofapython3
