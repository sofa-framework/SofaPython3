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
    - paul.scheikl@kit.edu
********************************************************************/

#pragma once

namespace sofapython3::doc::SofaDeformable {

static auto SpringForceFieldClass =
R"(
A ForceField containing LinearSpring.
)";

static auto SpringForceFieldClear =
R"(
Removes all springs and optionally reserve memory for n springs in the vector of springs.
:param reserve: reserve memory for n springs in the vector of springs.
)";

static auto SpringForceFieldRemoveSpring =
R"(
Removes a particular spring in SpringForceField by index.
:param index: index of the spring that should be removed
)";

static auto SpringForceFieldRemoveSprings =
R"(
Removes a number of spring specified by a list of indices
:param indices: list of indices specifying the springs that should be removed
)";

static auto SpringForceFieldGetSprings =
R"(
Returns all springs currently stored in the SpringForceField.
:rtype: List of LinearSpring
)";

static auto SpringForceFieldAddSpring =
R"(
Adds a new spring to the SpringForceField. Can be done either by passing an existing Sofa.SofaDeformable.LinearSpring object, or by passing arguments to construct a new LinearSpring.

examples:
------------

# Adding an existing spring to the force field
spring_force_field = node.addObject("SpringForceField")
spring = Sofa.SofaDeformable.LinearSpring(index1=0, index2=0, springStiffness=100, dampingFactor=5, restLength=5)
spring_force_field.addSpring(spring)

# Adding a new spring to the force field
spring_force_field = node.addObject("SpringForceField")
spring_force_field.addSpring(index1=0, index2=0, springStiffness=100, dampingFactor=5, restLength=5)
)";


static auto SpringForceFieldAddSprings =
R"(
Like addSpring, but for a list of springs.

examples:
------------

spring_force_field = node.addObject("SpringForceField")
springs = [Sofa.SofaDeformable.LinearSpring(index1=i, index2=i, springStiffness=100, dampingFactor=5, restLength=5) for i in range(1)]
spring_force_field.addSprings(springs)

)";

} // sofapython3::doc::SofaDeformable
