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

namespace sofapython3::doc::baseObject
{
static auto Class =
        R"(
        The class object is a base class for simulation components.

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

static auto getSlaves =
        R"(
        Return the slaves of the baseObject.
        :rtype: list
        )";

static auto getContext =
        R"(
        Return the conext of the baseObject.
        :rtype: BaseContext
        )";

static auto getMaster =
        R"(
        Return the master of the baseObject.
        :rtype: BaseObject
        )";

static auto addSlave =
        R"(
        Add a slave to the master BaseObject.
        :param slave: the slave to be added.
        :type slave: BaseObject
        )";

static auto storeResetState =
        R"(
        Save the initial state for later uses in reset()
        )";

static auto reset =
        R"(
        Reset to initial state.
        To save the initial state, see storeResetState().
        )";

static auto getName =
        R"(
        Accessor to the baseObject name.
        :rtype: string
        )";

static auto getTarget =
        R"(
        Return the target (plugin) that contains the current baseObject.
        :rtype: string
        )";

static auto getCategories =
        R"(
        Return the list of categories the current object belongs to.
        :rtype: list
        )";
static auto setSrc =
        R"(
        Set a source Object and parses it to collect dependent Data.
        )";

static auto bwdInit =
        R"(
        Initialization method called at graph creation and modification, during bottom-up traversal.
        )";

static auto cleanup =
        R"(
        Called just before deleting this object
        Any object in the tree bellow this object that are to be removed will be removed only after this call,
        so any references this object holds should still be valid.
        )";

static auto computeBBox =
        R"(
        Bounding Box computation method.
        Default to empty method.
        )";

static auto getLinkPath =
        R"(
        Return the full path name of this baseObject with an extra prefix '@'
        :rtype: string
        )";

static auto getAsACreateObjectParameter =
        R"(
        Return the full path name of this baseObject with an extra prefix '@'
        :rtype: string
        )";

static auto __getitem__ =
        R"(
        gets an item using its path (path is dot-separated, relative to the object
        it's called upon & ONLY DESCENDING (no ../):

        This method lifts most ambiguities when accessing a node, object or data
        from a path relative to self.

        examples:
        ------------

        root["node1.node2.object1.value"]

        In the example above, node1 and node2 can be inferred as being nodes without performing any checks.
        object1 can be a node or an object, but cannot be a datafield nor a link
        value can be a node or an object (if object1 is a node), or must be a data (if object1 is an object)
        )";

}
