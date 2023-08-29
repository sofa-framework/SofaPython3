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

namespace sofapython3::doc::forceField
{
static auto forceFieldClass = R"(
                         An overridable class to create your own customized force field
                         )";

static constexpr const char* assembleKMatrix = R"(
Assemble the stiffness matrix of a force field.

Note that this function is not free. It assembles the stiffness matrix whether or not it has been assembled previously
to add it into the global system matrix. Besides, the function does not prevent side effects of a second matrix assembly
in a single time step.

Typical usage example:
FEM = root.addObject('HexahedronFEMForceField', name="FEM", youngModulus="4000", poissonRatio="0.3", method="large")
...
stiffness_matrix = self.force_field.assembleKMatrix()

Returns:
    A scipy.sparse.csr_matrix object representing the stiffness matrix of the force field
)";
}
