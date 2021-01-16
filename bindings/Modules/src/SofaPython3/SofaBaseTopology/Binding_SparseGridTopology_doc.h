/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2019 CNRS, University of Lille, INRIA                  *
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
* Authors: see Authors.txt                                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
    - bruno.josue.marques@inria.fr
    - eve.le-guillou@centrale.centralelille.fr
    - jean-nicolas.brunet@inria.fr
    - thierry.gaugry@inria.fr
********************************************************************/

#pragma once

namespace sofapython3::doc::SofaBaseTopology {

static auto sparseGridTopologyClass =
R"(
A sparse grid topology. Like a sparse FFD building from the bounding box of the object. Starting from a RegularGrid,
only valid cells containing matter (ie intersecting the original surface mesh or totally inside the object) are
considered. Valid cells are tagged by a Type BOUNDARY or INSIDE.

WARNING: the corresponding node in the XML file has to be placed BEFORE the MechanicalObject node, in order to excute
its init() before the MechanicalObject one in order to be able to give dofs
)";

static auto getRegularGrid =
R"(
Get the underneath regular grid object of this sparse grid.

:rtype: RegularGridTopology
)";

static auto findCube =
R"(
Returns the cube containing the given point (or -1 if not found), as well as deplacements from its first corner in terms
of dx, dy, dz (i.e. barycentric coordinates).

:return: (cube_id, u, v, w)
:rtype: tuple
)";

static auto getRegularGridNodeIndex =
R"(
Get the index of a given node in the underneath regular grid.

:param index: The index of the node in the sparse grid.

:return: The index of the node in the regular grid.
:rtype: int
)";

static auto getRegularGridCubeIndex =
R"(
Get the index of a given cube in the underneath regular grid.

:param index: The index of the cube in the sparse grid.

:return: The index of the cube in the regular grid.
:rtype: int
)";

static auto getBoundaryCells =
R"(
Get the indices of the cells in this grid that lie on the boundary (i.e. intersect the surface mesh).
:rtype: list
)";

static auto getBoundaryNodes =
R"(
Get the indices of the nodes that forms the boundary of the grid (i.e. have less than 7 connected cells).
:rtype: list
)";

static auto getNodeIndicesOfCube =
R"(
Get the indices of the nodes of a given cube (grid cell).

:param index: The index of the cube.

:return: A list containing the indices of the cube nodes
:rtype: list
)";

} // sofapython3::doc::SofaBaseTopology
