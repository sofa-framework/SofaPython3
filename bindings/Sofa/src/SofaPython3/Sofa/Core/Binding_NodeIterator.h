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

#pragma once
#include <functional>
#include <pybind11/pybind11.h>

#include <sofa/simulation/Node.h>

namespace sofapython3 {

/// Generic implementation to iterator over the different structures of a Node.
/// This allows to implement pythonic solution as in:
///     for object in node.objects:
///          print(object.name)
///
/// or
///     for child in node.children:
///          print(child.name)
///
/// there is also support for several function like:
///     len(node.children)
///     if "MyName" in node.children
///     node.children.remove_at(index)
///
/// The implementation is generic by the use of lamda function allowing to specify which
/// data structure is actually iterated.
class NodeIterator
{
public:
    sofa::core::sptr<sofa::simulation::Node> owner;
    size_t     index=0;
    std::function<size_t (sofa::simulation::Node *)> size ;
    std::function<sofa::core::sptr<sofa::core::Base> (sofa::simulation::Node *, size_t)> get ;
    std::function<sofa::core::Base* (const sofa::simulation::Node *, const std::string&)> get_by_name ;
    std::function<void (sofa::simulation::Node*, unsigned int)> remove_at;

    NodeIterator(sofa::core::sptr<sofa::simulation::Node> owner_,
                 std::function<size_t (sofa::simulation::Node *)> size_,
                 std::function<sofa::core::sptr<sofa::core::Base> (sofa::simulation::Node *, size_t)> get_,
                 std::function<sofa::core::Base* (const sofa::simulation::Node *, const std::string&)> get_by_name_,
                 std::function<void (sofa::simulation::Node*, unsigned int)> remove_at_)
    {
        size = size_;
        get = get_;
        get_by_name = get_by_name_;
        remove_at = remove_at_;
        owner=owner_;
        index=0;
    }
};

void moduleAddNodeIterator(pybind11::module &m);

} /// namespace sofapython3
