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

namespace sofapython3::doc::datacontainer
{
static auto Class =
        R"(
        Classic python container implemented for SOFA data fields

        )";

static auto __len__ =
        R"(
        Return the number of elements in the container's 1st
        dimension. (equivalent to shape(0))
        )";

static auto size =
        R"(
        Return the total number of elements in the container
        ( equivalent to math.prod(self.shape()) )
        )";

static auto ndim =
        R"(
        Return the number of dimensions in the container
        )";

static auto shape =
        R"(
        Return the shape of the container (a tuple of length ndim,
        with each value being equal to the length of the given dimension)
        )";

}  // namespace sofapython3::doc::datacontainer
