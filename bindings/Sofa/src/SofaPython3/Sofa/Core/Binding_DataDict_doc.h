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

#pragma once

namespace sofapython3::doc::dataDict {

static auto Class =
        R"(
        DataDict exposes the data of a sofa object in a way similar to a normal python dictionnary.

        :Example:
            .. code-block:: python

                for k,v in anObject.__data__.items():
                print("Data name :"+k+" value:" +str(v)))
        )";

static auto keys =
        R"(
        Expose the data, but only the key (the name) of each items
        )";
static auto values =
        R"(
        Expose the data, but only the value of each items
        )";
static auto items =
        R"(
        Expose the data, both the key and the value of each item.
        :Example:
            .. code-block:: python

                for k,v in anObject.__data__.items():
                print("Data name :"+k+" value:" +str(v)))
        )";
}

namespace sofapython3::doc::dataDictIterator {

static auto Class =
        R"(

        Iterator reimplemented to fit dataDict.

        )";
}
