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

namespace sofapython3::doc::constraintsolver {

static auto constraintSolverClass =
R"(
Constraint solver.
)";

static auto constraintSolver_W =
R"(
Returns the compliance matrix projected in constraint space, built in the constraint solver

example:
------------

constraint_solver = root.addObject("ProjectedGaussSeidelConstraintSolver", tolerance=1e-9, maxIterations=1000)
matrix = constraint_solver.W()
)";

static auto constraintSolver_lambda =
R"(
Returns the force resulting from the constraints

example:
------------

constraint_solver = root.addObject("ProjectedGaussSeidelConstraintSolver", tolerance=1e-9, maxIterations=1000)
lambda = constraint_solver.lambda_force()
)";

static auto constraintSolver_dfree =
R"(
Returns the displacement computed without any constraint

example:
------------

constraint_solver = root.addObject("ProjectedGaussSeidelConstraintSolver", tolerance=1e-9, maxIterations=1000)
dfree = constraint_solver.dfree()
)";

} // namespace sofapython3::doc::constraintsolver
