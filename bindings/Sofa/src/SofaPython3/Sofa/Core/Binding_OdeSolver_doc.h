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

namespace sofapython3::doc::odesolver
{

static auto OdeSolverClass =
R"(
Component responsible for timestep integration, i.e. advancing the state from time t to t+dt.

This class currently control both the integration scheme (explicit,
implicit, static, etc), and the linear system resolution algorithm
(conjugate gradient, matrix direct inversion, etc). Those two aspect will
propably be separated in a future version.

While all computations required to do the integration step are handled by
this object, they should not be implemented directly in it, but instead
the solver propagates orders (or Visitor) to the other components in the
scenegraph that will locally execute them. This allow for greater
flexibility (the solver can just ask for the forces to be computed without
knowing what type of forces are present), as well as performances
(some computations can be executed in parallel).
)";


}
