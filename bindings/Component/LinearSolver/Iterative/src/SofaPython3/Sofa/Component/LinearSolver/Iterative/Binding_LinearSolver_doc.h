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

namespace sofapython3::doc::linearsolver {

static auto linearSolverClass =
R"(
Linear solver. Supports only scalar CompressedRowSparseMatrix.

example:
------------

linear_solver = root.addObject('SparseLDLSolver', template='CompressedRowSparseMatrixd') # supported
linear_solver = root.addObject('SparseLDLSolver', template='CompressedRowSparseMatrixMat3x3d') # not supported
)";

static auto linearSolver_A =
R"(
Returns the global system matrix as a scipy sparse matrix

example:
------------
linear_solver = root.addObject('SparseLDLSolver', template='CompressedRowSparseMatrixd')
matrix = linear_solver.A()
)";

static auto linearSolver_b =
R"(
Returns the global system right hand side as a numpy array

example:
------------
linear_solver = root.addObject('SparseLDLSolver', template='CompressedRowSparseMatrixd')
matrix = linear_solver.b()
)";

static auto linearSolver_x =
R"(
Returns the global system solution vector as a numpy array

example:
------------
linear_solver = root.addObject('SparseLDLSolver', template='CompressedRowSparseMatrixd')
matrix = linear_solver.x()
)";

} // namespace sofapython3::doc::baseCamera
