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

#include <pybind11/pybind11.h>
namespace py = pybind11;
using namespace pybind11::literals;


#include <sofa/type/Quat.h>

void moduleAddQuat(py::module& m);

namespace pyQuat
{
template <class T>
std::string __str__(const sofa::type::Quat<T> &self, bool repr = false)
{
    std::string s;
    if (repr)
    {
        s += "Quat";
    }
    s += "(";
    s += std::to_string(self[0])
            + ", " + std::to_string(self[1])
            + ", " + std::to_string(self[2])
            + ", " + std::to_string(self[3])
            + ")";
    return s;
}

} // namespace pyQuat
