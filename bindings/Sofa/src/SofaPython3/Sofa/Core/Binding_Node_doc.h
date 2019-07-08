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

static auto Class =
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
static auto init =
        R"(
        Initialize the components of this node and all the nodes which depend on it.
        :rtype: Sofa.Simulation.Node
        )";

static auto init1Arg =
        R"(
        Initialize the components of this node and all the nodes which depend on it.

        :param name: Name of the node to be created
        :type name: string
        :rtype: Sofa.Simulation.Node
        )";

static auto initSofa =
        R"(
        Initialize the components of this node and all the nodes which depend on it.
        (beware this is not the python __init__, this is sofa's init())

        :param self: the node to initialize
        :type self: Sofa.Simulation.Node&
        :rtype: Sofa.Simulation.Node
        )";

static auto addObjectKwargs =
        R"(
        Add an object.
        Detect the implemented interfaces and add the object to the corresponding lists.
        :param self: the node itself
        :param type: type of the object
        :param kwargs
        :type self: Sofa.Simulation.Node*
        :type type: string&
        :type kwargs: kwargs&
        )";

static auto addObject =
        R"(
        Add an object.
        Detect the implemented interfaces and add the object to the corresponding lists.
        :param self: the node itself
        :param object: the object to be added
        :type self: Sofa.Simulation.Node&
        :type object: Sofa.Simulation.BaseObject*
        )";


static auto createObject =
        R"(
        Deprecated, see addObject
        )";

static auto addChildKwargs =
        R"(
        Add a child node
        :param self: the node itself
        :param name: name of the child to be added
        :param kwargs
        :type self: Sofa.Simulation.Node*
        :type name: string&
        :type kwargs: kwargs&
        :rtype: Sofa.Simulation.Node
        )";

static auto addChild =
        R"(
        Add a child node

        :param self : the node itself
        :param child : the child to be added
        :type self: Sofa.Simulation.Node*
        :type child: Sofa.Simulation.Node*
        :rtype: Sofa.Simulation.Node
        )";

static auto createChild =
        R"(
        Deprecated, see addChild
        )";

static auto getChild =
        R"(
        Get the child of a node.

        :param n
        :param name
        :type n: Sofa.Simulation.Node
        :type name: string
        :return: the child of the same name
        )";

static auto removeChild =
        R"(
        Remove a child of a node.
        :param self: the node itself
        :param n: the child to remove
        :type self: Sofa.Simulation.Node&
        :type n: Sofa.Simulation.Node&
        :Examples:
        >>> node1.removeChild(node2)
        )";

static auto removeChildWithName =
        R"(
        Remove a child of a node.
        :param n: the node itself
        :param name: the name of the child to remove
        :type n: Sofa.Simulation.Node&
        :type name: string
        :Examples:
        >>> node1.removeChild("nameNode2")
        )";

static auto getRoot =
        R"(
        Get the root node of the current node.
        :rtype: Sofa.Simulation.BaseNode*
        )";

static auto getPathName =
        R"(
        Get the path name of the current node
        :rtype: string
        )";

static auto getLink =
        R"(
        Get the link of the current node
        :param node
        :type node: Sofa.Simulation.Node*
        )";

static auto children =
        R"(
        Field interface to acces the children of a node.

        :Example:
        >>> n = Sofa.Core.Node("MyNode"")
        >>> for child in n.children:
        >>>     print(child.name)
        )";

static auto parents =
        R"(
        Field interface to acces the parents of a node.
        :Example:
        >>> n = Sofa.Core.Node("MyNode"")
        >>> for parent in n.parents:
        >>>     print(parent.name)
        )";
static auto objects =
        R"(
        Field interface to acces the objects of a node.

        :Example:
        >>> n = Sofa.Core.Node("MyNode"")
        >>> for object in n.objects:
        >>>     print(object.name)
        )";

}



