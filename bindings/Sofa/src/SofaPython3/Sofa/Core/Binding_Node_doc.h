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

namespace sofapython3::doc::sofa::core::Node
{

static auto Class =
        R"(
        A Node stores other nodes and components.

        A simulation in SOFA is described as a scene with an intrinsic generalized hierarchy.
        A scene is composed of nodes organized as a tree or as a Directed Acyclic Graph (DAG).
        The different simulated objects are described in separate nodes, and different representations
        of a same object can be done in different sub-nodes.

        Example of use:
          .. code-block:: python

                import Sofa.Core

                # Create a new node
                n = Sofa.Core.Node("MyNode")

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
             n = Sofa.Core.Node("MyNode")
             for child in n.children:
                 print(child.name)

             for parent in n.parents:
                 print(parent.name)


        Accessing children or object from the current node is easy thanks to the generalized access
        API.
        Example of use:
        .. code-block:: python

             # ...
             n = Sofa.Core.Node("MyNode")
             c1 = n.addChild("child1")
             c2 = n.addChild("child2")
             c2.addObject("MechanicalObject", name="dofs")

             # retrieve the node child1/child2 from the 'n' node.
             n.child1.child2

             # retrieve the child1/child2/dofs.position position data field from 'n'
             n.child1.child2.dofs.position

             # fast access.
             n["child1.child2.dofs.position"]

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

static auto addKwargs =
        R"(
        Add an prefab,
        example:
            def Cube(parentNodes, name="Cube"):
                cube = parentNode.addChild(name)
                cube.addObject("MechanicalObject")
                return cube

            node.add(Cube, name="MyCube"")
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

static auto hasObject =
        R"(
        Check if there is a component with provided name.

        :param n
        :param name
        :type n: Sofa.Simulation.Node
        :type name: string
        :return: True if the node has an object with correspdonding name.
        )";
static auto getObject =
        R"(
        Get a sofa component hold by a node.

        :param n
        :param name
        :type n: Sofa.Simulation.Node
        :type name: string
        :return: the component with 'name', None otherwise

        .. note::
        The extra arguments allowed in the SofaPython (warning=True/False) binding are not supported SofaPython3.

        .. code-block:: python
            # SofaPython2:
            if node.getObject("MyObject",warning=False):
                pass

            # SofaPython3:
            if node.getObject("MyObject") != None:
                pass

            if node.hasObject("MyObject"):
                pass

            if "MyObject" in node.objects:
                pass
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
        :return: the child with 'name', None otherwise
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

static auto getLinkPath =
        R"(
        Get the link of the current node
        :param node
        :type node: Sofa.Simulation.Node*
        )";

static auto children =
        R"(
        Field interface to acces the children of a node.
        The returned object is a iteratable featuring the following operations:
        len, remove_at, __contains__, get_at

        :Example:
        >>> n = Sofa.Core.Node("MyNode")
        >>> n.addChild("child1")
        >>> for child in n.children:
        >>>     print(child.name)
        >>>
        >>> if "child1" in n.children:
        >>>     print("Yes")
        >>> print(len(n.children))
        )";

static auto parents =
        R"(
        Field interface to acces the parents of a node.
        The returned object is a iteratable featuring the following operations:
        len, remove_at, __contains__, get_at

        :Example:
        >>> n = Sofa.Core.Node("parent1")
        >>> c = n.addChild("child1")
        >>> for parent in c.parents:
        >>>     print(parent.name)
        >>>
        >>> if "parent1" in c.parents:
        >>>     print("Yes")
        >>> print(len(n.parents))
        )";
static auto objects =
        R"(
        Field interface to acces the objects of a node.
        The returned object is a iteratable featuring the following operations:
        len, remove_at, __contains__, get_at

        :Example:
        >>> n = Sofa.Core.Node("MyNode")
        >>> n.addObject("MechanicalObject", name="object1")
        >>> n.addObject("MechanicalObject", name="object2")
        >>> for object in n.objects:
        >>>     print(object.name)
        >>>
        >>> if "object2" in c.objects:
        >>>     print("Yes")
        >>> print(len(n.objects))
        )";
static auto removeObject =
        R"(
        Remove an object
        :param object: the object to be removed
        :type object: BaseObject
        )";

static auto getRootPath =
        R"(
        Return the path from this node to the root node
        )";

static auto moveChild =
        R"(
        Move a node from another node.
        :param child: the node to be moved
        :param prevParent: the previous parent of the node to be moved
        :type child: Sofa.Simulation.Node
        :type prevParent: Sofa.Simulation.Node
        )";

static auto isInitialized =
        R"(
        Checks if the node has been initialized
        :return: true if it has been initialized
        )";
static auto getAsACreateObjectParameter =
        R"(
        Get the link of the current node
        :rtype: string
        )";

static auto detachFromGraph =
        R"(
        Remove the current node from the graph: depending on the type of Node, it can have one or several parents.
        )";

static auto getMass =
        R"(
        Get the mass of the node
        )";
static auto getForceField =
        R"(
        Get the force field of a node, given an index.
        :param index: index of the force field
        :type index: unsigned int.
        )";

static auto getMechanicalState =
        R"(
        Get the mechanical state of the node.
        )";

static auto hasODESolver =
        R"(
        Return true of the node includes an ODE Solver
        )";

static auto getMechanicalMapping =
        R"(
        Get the mechanical mapping of the node.
        )";
static auto sendEvent =
        R"(
        Send an event to other nodes,
        by creating a PythonScriptEvent and propagating it to the rest of the tree.
        Only the nodes and objects downstream will receive the message.
        :param pyUserData: the user data that can be sent
        :param eventName: the name of the event
        :type pyUserData: py::object
        :type eventName: string
        )";

}



