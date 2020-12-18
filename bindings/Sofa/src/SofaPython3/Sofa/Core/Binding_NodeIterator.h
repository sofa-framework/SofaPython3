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

#pragma once
#include <functional>
#include <pybind11/pybind11.h>

#include <sofa/simulation/Node.h>

namespace sofapython3 {

class NodeIterator {
public:
    sofa::core::sptr<sofa::simulation::Node> owner;
    size_t     index=0;
    std::function<size_t (sofa::simulation::Node *)> size ;
    std::function<sofa::core::sptr<sofa::core::Base> (sofa::simulation::Node *, size_t)> get ;

    NodeIterator(sofa::core::sptr<sofa::simulation::Node> owner_,
                 std::function<size_t (sofa::simulation::Node *)> size_,
                 std::function<sofa::core::sptr<sofa::core::Base> (sofa::simulation::Node *, size_t)> get_)
    {
        size = size_;
        get = get_;
        owner=owner_;
        index=0;
    }
};

void moduleAddNodeIterator(pybind11::module &m);

} /// namespace sofapython3
