/*********************************************************************
Copyright 2019, Inria, CNRS, University of Lille

This file is part of runSofa2

runSofa2 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

runSofa2 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#pragma once

namespace sofapython3::doc::baseObject
{
static auto Class =
        R"(
        Base class for simulation components.

        An object defines a part of the functionnality in the simulation
        (stores state data, specify topology, compute forces, etc).
        Each simulation object is related to a context, which gives access to all available external data.
        It is able to process events, if listening enabled (default is false).
        )";
static auto init =
        R"(
        Initialization method called at graph creation and modification, during top-down traversal.Initialize data.
        )";

static auto reinit =
        R"(
        Update method called when variables used in precomputation are modified.
        )";
static auto getPathName =
        R"(
        Return the full path name of this baseObject
        :rtype: string
        )";

static auto getLink =
        R"(
        Return the link of the baseObject
        :param self: the baseObject itself
        :type self: baseObject
        :rtype: string
        )";
}
