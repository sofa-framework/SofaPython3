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

namespace sofapython3::doc::dataengine
{
static auto DataEngine =
        R"(
        Class computing a set of Data outputs from a set of Data inputs

        It is a trampoline class that enables python implementations
        of the C++ sofa::core::DataEngine class
        )";

static auto addInput =
        R"(
        Registers a data as an input for the DataEngine.

        :param data: The data to register as input.
        :type data: Sofa.Core.Data.
        :returns:  None.
        )";
//                .. function:: addInput(data: Data)
//        :param data: the data to register as an input
//        :rtype: None
//        registers a data field as being an input for this engine.
//        Changing an input's value flags the engine as dirty,
//        which means that any access to its outputs will trigger a call to update()

static auto addOutput =
        R"(
        Registers a data field as being an output for this engine.
        If an Engine's output is accessed (getValue, beginEdit...)
        AND one or more of its input is dirty, the engine's update()
        method will be called
        :param data: the data to register as an output
        :rtype: None
        )";

}  // namespace sofapython3::doc::dataengine
