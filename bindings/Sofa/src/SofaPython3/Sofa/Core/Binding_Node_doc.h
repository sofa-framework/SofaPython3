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
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
********************************************************************/
#pragma once

namespace sofapython3::doc::sofa::core::Node
{

static auto docstring =
        R"(
        Node in the scene graph
        ---------------

        A Node stores other nodes and components.

        Example of use:
          .. code-block:: python

             import Sofa

             # Create a new node
             n = Sofa.Core.Node("MyNode"")

             # Add a child node name 'Node2'
             n.addChild("Node2")

             # Add a mechanical component to MyNode
             n.addObject("MechanicalObject", name="dofs")

             Sofa.Simulation.init(root)
             Sofa.Simulation.print(root)

        The child nodes, components and parents can be accessed using generator attributes.

        Example of use:
          .. code-block:: python

             # ...
             n = Sofa.Core.Node("MyNode"")
             for child in n.children:
                 print(child.name)

             for parent in n.parents:
                 print(parent.name)


        Accessing children or object from the current node is easy thanks to the generalized access
        API.
        Example of use:
          .. code-block:: python

             # ...
             n = Sofa.Core.Node("MyNode"")
             c1 = n.addChild("child1")
             c2 = n.addChild("child2")
             c2.addObject("MechanicalObject", name="dofs")

             # retrieve the node child1/child2 from the 'n' node.
             n.child1.child2

             # retrieve the child1/child2/dofs.position position data field from 'n'
             n.child1.child2.dofs.position

             # fast access.
             n["child1.child2.dofs.position"]


        .. autoclass:: Sofa.Node
        :members:
        :undoc-members:
        )";



}
