"""
           Scene components
           -----------------------

           Core components
           ^^^^^^^^^^^^^^^

           .. autosummary::
               :toctree: _autosummary/_autosummary

               Sofa.Core.Data
               Sofa.Core.Link
               Sofa.Core.Node
               Sofa.Core.RawPrefab
               Sofa.Core.Object
               Sofa.Core.Camera

           Overridable components
           ^^^^^^^^^^^^^^^^^^^^^^

           .. autosummary::
               :toctree: _autosummary/_autosummary

               Sofa.Core.Controller
               Sofa.Core.ForceField
               Sofa.Core.DataEngine


           Utilitary classes
           ^^^^^^^^^^^^^^^^^

           .. autosummary::
               :toctree: _autosummary/_autosummary

               Sofa.Core.DataDict
               Sofa.Core.DataDictIterator
               Sofa.Core.DataContainer
               Sofa.Core.DataString
               Sofa.Core.DataVectorString
               Sofa.Core.NodeIterator
               #Sofa.Core.WriteAccessor
       """
import Sofa.Core
import typing
import numpy
_Shape = typing.Tuple[int, ...]

__all__ = [
    "
    Record storing information about a class
    ",
    "Base",
    "BaseContext",
    "BaseMeshTopology",
    "BaseNode",
    "BasePrefab",
    "Camera",
    "ContactListener",
    "Context",
    "Controller",
    "Data",
    "DataContainer",
    "DataContainerContextManager",
    "DataDict",
    "DataDictIterator",
    "DataEngine",
    "DataLink",
    "DataString",
    "DataVectorString",
    "ForceFieldRigid2d",
    "ForceFieldRigid3d",
    "ForceFieldVec1d",
    "ForceFieldVec2d",
    "ForceFieldVec3d",
    "Link",
    "Node",
    "NodeIterator",
    "Object",
    "ObjectFactory",
    "PrefabLink",
    "RawPrefab",
    "Topology"
]


class 
    Record storing information about a class
    ():
    @property
    def aliases(self) -> None:
        """
        :type: None
        """
    @property
    def authors(self) -> None:
        """
        :type: None
        """
    @property
    def className(self) -> None:
        """
        :type: None
        """
    @property
    def dataAlias(self) -> None:
        """
        :type: None
        """
    @property
    def defaultTemplate(self) -> None:
        """
        :type: None
        """
    @property
    def description(self) -> None:
        """
        :type: None
        """
    @property
    def license(self) -> None:
        """
        :type: None
        """
    @property
    def locations(self) -> None:
        """
        :type: None
        """
    @property
    def targets(self) -> None:
        """
        :type: None
        """
    @property
    def templates(self) -> None:
        """
        :type: None
        """
    pass
class Base():
    """
            Sofa.Core.Base is the root of the Sofa class Hierarhcy
            All objects are in-heriting from this one.
            
    """
    def __getattr__(self, arg0: str) -> object: ...
    def __setattr__(self, arg0: str, arg1: object) -> None: ...
    @staticmethod
    def addData(*args, **kwargs) -> typing.Any: 
        """
                Create a data field, then adds it to the base.
                Note that this method should only be called if the field was not initialized with the initData method
                :param self: the base itself
                :param name: the name of the data to be added
                :param value: the value from which the data can be created
                :param help: help message that describes the data to be created
                :param group: the group the data belongs to
                :param type: the type of the data
                :type self: object
                :type name: string
                :type value: object
                :type help: string
                :type group: string
                :type type: string
                


                Add a data field.
                Note that this method should only be called if the field was not initialized with the initData method
                :param self: the base itself
                :param d: the data to be added
                :type self: Base*
                :type d: object
                
        """
    @staticmethod
    def addLink(*args, **kwargs) -> typing.Any: 
        """
                Create a Link to a SOFA component and adds it to the base.
                Note that this method should only be called if the field was not initialized with the initLink method
                :param self: the base itself
                :param name: the name of the link to be added
                :param value: the value from which the data can be created (either a pathname or a SofaBase)
                :param help: help message that describes the link to be created
                :type self: object
                :type name: string
                :type value: object
                :type help: string
                
        """
    def clearLoggedMessages(self) -> object: 
        """
                Remove all logged messages in the object's logs.
                
        """
    def countLoggedMessages(self) -> object: 
        """
                Returns the number of messages in the object's logs.
                
        """
    @staticmethod
    def findData(*args, **kwargs) -> typing.Any: 
        """
                Find a data field given its name.
                Return NULL if not found.
                If more than one field is found (due to aliases), only the first is returned.
                :param name
                :type name: string
                :return: the data field
                
        """
    @staticmethod
    def findLink(*args, **kwargs) -> typing.Any: 
        """
                Find a link given its name.
                Return NULL if not found.
                If more than one link is found (due to aliases), only the first is returned.
                :param name: the name of the link
                :type name: string
                :return: the link
                
        """
    @staticmethod
    def getClass(*args, **kwargs) -> typing.Any: 
        """
                Return the class of the object
                
        """
    def getClassName(self) -> str: 
        """
                Get the name of the class of the Base.
                
        """
    def getData(self, arg0: str) -> object: 
        """
                Get the data field given its name.
                :param self:
                :param s:
                :type self: Base&
                :type s: string
                :return: the first data found of this name
                
        """
    def getDataFields(self) -> list: 
        """
                Accessor to the vector containing all the fields of this object
                :return: A vector containing the data fields
                
        """
    def getDefinitionSourceFileName(self) -> str: 
        """
        Returns the name of the file that contains the object definition.
        """
    def getDefinitionSourceFilePos(self) -> int: 
        """
        Returns the line number where the object is defined.
        """
    def getInstanciationFileName(self) -> str: 
        """
        Returns the line number where the object is instanciatiated.
        """
    def getInstanciationSourceFilePos(self) -> int: 
        """
        Returns the line number where the object is instanciatiated.
        """
    def getLinks(self) -> list: 
        """
                Accessor to the vector containing all the links of this object
                :return: A vector containing the links
                
        """
    def getLoggedMessagesAsString(self) -> object: 
        """
                Returns a single string with all the messages logged in the internal buffer of a sofa object.
                Return empty string if there is no messages.
                
        """
    def getName(self) -> str: 
        """
                Return the name of the entity
                :rtype: string
                
        """
    def getPathName(self) -> str: ...
    def getTemplateName(self) -> str: 
        """
                Get the name of the template of the Base.
                
        """
    def setDataValues(self, **kwargs) -> object: 
        """
                Set values for a the given data field, multiple pairs of args are allowed.
                
        """
    def setDefinitionSourceFileName(self, arg0: str) -> None: 
        """
        set the name of the file that contains the object definition.
        """
    def setDefinitionSourceFilePos(self, arg0: int) -> None: 
        """
        Set the line number where the object is defined.
        """
    def setInstanciationSourceFileName(self, arg0: str) -> None: 
        """
        Set the line number where the object is instanciatiated.
        """
    def setInstanciationSourceFilePos(self, arg0: int) -> None: 
        """
        Set the line number where the object is instanciatiated.
        """
    def setName(self, arg0: str) -> None: 
        """
                Set the name of this object
                :param n
                :type n: string
                
        """
    pass
class BaseContext(Base):
    class SearchDirection():
        """
        Members:

          SearchUp

          Local

          SearchDown

          SearchRoot

          SearchParents
        """
        def __eq__(self, other: object) -> bool: ...
        def __getstate__(self) -> int: ...
        def __hash__(self) -> int: ...
        def __index__(self) -> int: ...
        def __init__(self, value: int) -> None: ...
        def __int__(self) -> int: ...
        def __ne__(self, other: object) -> bool: ...
        def __repr__(self) -> str: ...
        def __setstate__(self, state: int) -> None: ...
        @property
        def name(self) -> str:
            """
            :type: str
            """
        @property
        def value(self) -> int:
            """
            :type: int
            """
        Local: Sofa.Core.BaseContext.SearchDirection # value = <SearchDirection.Local: 0>
        SearchDown: Sofa.Core.BaseContext.SearchDirection # value = <SearchDirection.SearchDown: 1>
        SearchParents: Sofa.Core.BaseContext.SearchDirection # value = <SearchDirection.SearchParents: 3>
        SearchRoot: Sofa.Core.BaseContext.SearchDirection # value = <SearchDirection.SearchRoot: 2>
        SearchUp: Sofa.Core.BaseContext.SearchDirection # value = <SearchDirection.SearchUp: -1>
        __members__: dict # value = {'SearchUp': <SearchDirection.SearchUp: -1>, 'Local': <SearchDirection.Local: 0>, 'SearchDown': <SearchDirection.SearchDown: 1>, 'SearchRoot': <SearchDirection.SearchRoot: 2>, 'SearchParents': <SearchDirection.SearchParents: 3>}
        pass
    def __str__(self) -> str: 
        """
        Get a string representation of the context.
        """
    def canChangeSleepingState(self) -> bool: 
        """
        Whether the context can change its sleeping state or not
        """
    def getAnimate(self) -> bool: 
        """
        Animation flag
        """
    def getDt(self) -> float: 
        """
        Simulation timestep
        """
    @staticmethod
    def getGravity(*args, **kwargs) -> typing.Any: 
        """
        Gravity in local coordinates
        """
    @staticmethod
    def getMass(*args, **kwargs) -> typing.Any: 
        """
        Mass
        """
    @staticmethod
    def getMechanicalState(*args, **kwargs) -> typing.Any: 
        """
        Mechanical Degrees-of-Freedom
        """
    @staticmethod
    def getMeshTopology(*args, **kwargs) -> typing.Any: 
        """
        Mesh Topology (unified interface for both static and dynamic topologies)
        """
    @staticmethod
    def getMeshTopologyLink(*args, **kwargs) -> typing.Any: 
        """
        Mesh Topology (unified interface for both static and dynamic topologies)
        """
    def getRootContext(self) -> BaseContext: 
        """
        Get the root context of the graph
        """
    @staticmethod
    def getState(*args, **kwargs) -> typing.Any: 
        """
        Mechanical Degrees-of-Freedom
        """
    def getTime(self) -> float: 
        """
        Simulation time
        """
    @staticmethod
    def getTopology(*args, **kwargs) -> typing.Any: 
        """
        Topology
        """
    def isActive(self) -> bool: 
        """
        The Context is active
        """
    def isSleeping(self) -> bool: 
        """
        Sleeping state of the context
        """
    def setActive(self, arg0: bool) -> None: 
        """
        State of the context
        """
    def setAnimate(self, arg0: bool) -> None: 
        """
        Animation flag
        """
    def setChangeSleepingState(self, arg0: bool) -> None: 
        """
        Sleeping state change of the context
        """
    def setDt(self, arg0: float) -> None: 
        """
        Simulation timestep
        """
    @staticmethod
    def setGravity(*args, **kwargs) -> typing.Any: 
        """
        Gravity in local coordinates
        """
    def setSleeping(self, arg0: bool) -> None: 
        """
        Sleeping state of the context
        """
    Local: Sofa.Core.BaseContext.SearchDirection # value = <SearchDirection.Local: 0>
    SearchDown: Sofa.Core.BaseContext.SearchDirection # value = <SearchDirection.SearchDown: 1>
    SearchParents: Sofa.Core.BaseContext.SearchDirection # value = <SearchDirection.SearchParents: 3>
    SearchRoot: Sofa.Core.BaseContext.SearchDirection # value = <SearchDirection.SearchRoot: 2>
    SearchUp: Sofa.Core.BaseContext.SearchDirection # value = <SearchDirection.SearchUp: -1>
    pass
class BaseMeshTopology(Base):
    def getEdge(self, index: int) -> typing.List[int[2]]: ...
    def getEdgesInTetrahedron(self, index: int) -> typing.List[int[6]]: 
        """
        Returns the set of edges adjacent to a given tetrahedron.
        """
    def getLocalEdgesInTetrahedron(self, index: int) -> typing.List[int[2]]: 
        """
        Returns for each index (between 0 and 5) the two vertex indices that are adjacent to that edge.
        """
    def getNbEdges(self) -> int: ...
    def getNbHexahedra(self) -> int: ...
    def getNbLines(self) -> int: ...
    def getNbPoints(self) -> int: ...
    def getNbQuads(self) -> int: ...
    def getNbTetrahedra(self) -> int: ...
    def getNbTetras(self) -> int: ...
    def getNbTriangles(self) -> int: ...
    def getTetrahedron(self, index: int) -> typing.List[int[4]]: 
        """
        Returns the vertices of Tetrahedron at index.
        """
    pass
class BaseNode(Base):
    pass
class Context(BaseContext, Base):
    pass
class Object(Base):
    """
            The class object is a base class for simulation components.

            An object defines a part of the functionnality in the simulation
            (stores state data, specify topology, compute forces, etc).
            Each simulation object is related to a context, which gives access to all available external data.
            It is able to process events, if listening enabled (default is false).

            :Example of use:

            .. code-block:: python

                import Sofa.Core
                import SofaRuntime
                SofaRuntime.importPlugin("SofaComponentAll")

                root = Sofa.Core.Node("root")
                root.addObject("MechanicalObject", name="obj") # Add a common Sofa object
                root.obj # Access the object
                root.obj.name # Access the name of the object
                root.obj.position.value # Access the position of the object

            
    """
    def __getitem__(self, arg0: str) -> object: 
        """
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
                
        """
    def addSlave(self, arg0: Object) -> None: 
        """
                Add a slave to the master BaseObject.
                :param slave: the slave to be added.
                :type slave: BaseObject
                
        """
    def bwdInit(self) -> None: 
        """
                Initialization method called at graph creation and modification, during bottom-up traversal.
                
        """
    def cleanup(self) -> None: 
        """
                Called just before deleting this object
                Any object in the tree bellow this object that are to be removed will be removed only after this call,
                so any references this object holds should still be valid.
                
        """
    @staticmethod
    @typing.overload
    def computeBBox(*args, **kwargs) -> typing.Any: 
        """
                Bounding Box computation method.
                Default to empty method.
                


                Bounding Box computation method.
                Default to empty method.
                
        """
    @typing.overload
    def computeBBox(self) -> None: ...
    def getAsACreateObjectParameter(self) -> str: 
        """
                Return the full path name of this baseObject with an extra prefix '@'
                :rtype: string
                
        """
    def getCategories(self) -> object: 
        """
                Return the list of categories the current object belongs to.
                :rtype: list
                
        """
    def getContext(self) -> object: 
        """
                Return the conext of the baseObject.
                :rtype: BaseContext
                
        """
    def getLinkPath(self) -> str: 
        """
                Return the link of the baseObject
                :param self: the baseObject itself
                :type self: baseObject
                :rtype: string
                


                Return the full path name of this baseObject with an extra prefix '@'
                :rtype: string
                
        """
    def getMaster(self) -> object: 
        """
                Return the master of the baseObject.
                :rtype: BaseObject
                
        """
    def getPathName(self) -> str: 
        """
                Return the full path name of this baseObject
                :rtype: string
                
        """
    def getSlaves(self) -> list: 
        """
                Return the slaves of the baseObject.
                :rtype: list
                
        """
    def getTarget(self) -> object: 
        """
                Return the target (plugin) that contains the current baseObject.
                :rtype: string
                
        """
    def init(self) -> None: 
        """
                Initialization method called at graph creation and modification, during top-down traversal.Initialize data.
                
        """
    def reinit(self) -> None: 
        """
                Update method called when variables used in precomputation are modified.
                
        """
    def reset(self) -> None: 
        """
                Reset to initial state.
                To save the initial state, see storeResetState().
                
        """
    def setSrc(self, arg0: str, arg1: Object) -> None: 
        """
                Set a source Object and parses it to collect dependent Data.
                
        """
    def storeResetState(self) -> None: 
        """
                Save the initial state for later uses in reset()
                
        """
    pass
class ContactListener(Object, Base):
    """
            ContactListener is an API dedicated to the monitoring of two collision models.

            :Example of use:

            .. code-block:: python

                import Sofa.Core
                import SofaRuntime
                SofaRuntime.importPlugin("SofaComponentAll")

                root = Sofa.Core.Node("root")
                model1 = root.addObject("TriangleCollisionModel")
                model2 = root.addObject("PointCollisionModel")
                listener = root.addObject(
                    "ContactListener",
                    collisionModel1=model1.getLinkPath(),
                    collisionModel2=model2.getLinkPath(),
                )
                print(listener.getNumberOfContacts())
            
    """
    pass
class Node(BaseNode, Context, BaseContext, Base):
    """
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

            
    """
    def __getattr__(self, arg0: str) -> object: ...
    def __getitem__(self, arg0: str) -> object: ...
    @typing.overload
    def __init__(self) -> None: 
        """
                Initialize the components of this node and all the nodes which depend on it.
                :rtype: Sofa.Simulation.Node
                


                Initialize the components of this node and all the nodes which depend on it.

                :param name: Name of the node to be created
                :type name: string
                :rtype: Sofa.Simulation.Node
                
        """
    @typing.overload
    def __init__(self, name: str) -> None: ...
    def add(self, arg0: object, **kwargs) -> object: 
        """
                Add an prefab,
                example:
                    def Cube(parentNodes, name="Cube"):
                        cube = parentNode.addChild(name)
                        cube.addObject("MechanicalObject")
                        return cube

                    node.add(Cube, name="MyCube"")
                
        """
    @typing.overload
    def addChild(self, arg0: Node) -> Node: 
        """
                Add a child node
                :param self: the node itself
                :param name: name of the child to be added
                :param kwargs
                :type self: Sofa.Simulation.Node*
                :type name: string&
                :type kwargs: kwargs&
                :rtype: Sofa.Simulation.Node
                


                Add a child node

                :param self : the node itself
                :param child : the child to be added
                :type self: Sofa.Simulation.Node*
                :type child: Sofa.Simulation.Node*
                :rtype: Sofa.Simulation.Node
                
        """
    @typing.overload
    def addChild(self, arg0: str, **kwargs) -> object: ...
    @typing.overload
    def addObject(self, arg0: Object) -> object: 
        """
                Add an object.
                Detect the implemented interfaces and add the object to the corresponding lists.
                :param self: the node itself
                :param type: type of the object
                :param kwargs
                :type self: Sofa.Simulation.Node*
                :type type: string&
                :type kwargs: kwargs&
                


                Add an object.
                Detect the implemented interfaces and add the object to the corresponding lists.
                :param self: the node itself
                :param object: the object to be added
                :type self: Sofa.Simulation.Node&
                :type object: Sofa.Simulation.BaseObject*
                
        """
    @typing.overload
    def addObject(self, arg0: str, **kwargs) -> object: ...
    def createChild(self, arg0: str, **kwargs) -> object: 
        """
                Deprecated, see addChild
                
        """
    def createObject(self, arg0: str, **kwargs) -> object: 
        """
                Deprecated, see addObject
                
        """
    def detachFromGraph(self) -> None: 
        """
                Remove the current node from the graph: depending on the type of Node, it can have one or several parents.
                
        """
    def getAsACreateObjectParameter(self) -> str: 
        """
                Get the link of the current node
                :rtype: string
                
        """
    def getChild(self, arg0: str) -> object: 
        """
                Get the child of a node.

                :param n
                :param name
                :type n: Sofa.Simulation.Node
                :type name: string
                :return: the child of the same name
                
        """
    def getForceField(self, arg0: int) -> object: 
        """
                Get the force field of a node, given an index.
                :param index: index of the force field
                :type index: unsigned int.
                
        """
    def getLinkPath(self) -> str: 
        """
                Get the link of the current node
                :param node
                :type node: Sofa.Simulation.Node*
                
        """
    def getMass(self) -> object: 
        """
                Get the mass of the node
                
        """
    def getMechanicalMapping(self) -> object: 
        """
                Get the mechanical mapping of the node.
                
        """
    def getMechanicalState(self) -> object: 
        """
                Get the mechanical state of the node.
                
        """
    def getPathName(self) -> str: 
        """
                Get the path name of the current node
                :rtype: string
                
        """
    def getRoot(self) -> object: 
        """
                Get the root node of the current node.
                :rtype: Sofa.Simulation.BaseNode*
                
        """
    def getRootPath(self) -> str: 
        """
                Return the path from this node to the root node
                
        """
    def init(self) -> None: 
        """
                Initialize the components of this node and all the nodes which depend on it.
                (beware this is not the python __init__, this is sofa's init())

                :param self: the node to initialize
                :type self: Sofa.Simulation.Node&
                :rtype: Sofa.Simulation.Node
                
        """
    def isInitialized(self) -> bool: 
        """
                Checks if the node has been initialized
                :return: true if it has been initialized
                
        """
    def moveChild(self, arg0: BaseNode, arg1: BaseNode) -> None: 
        """
                Move a node from another node.
                :param child: the node to be moved
                :param prevParent: the previous parent of the node to be moved
                :type child: Sofa.Simulation.Node
                :type prevParent: Sofa.Simulation.Node
                
        """
    @typing.overload
    def removeChild(self, arg0: Node) -> None: 
        """
                Remove a child of a node.
                :param self: the node itself
                :param n: the child to remove
                :type self: Sofa.Simulation.Node&
                :type n: Sofa.Simulation.Node&
                :Examples:
                >>> node1.removeChild(node2)
                


                Remove a child of a node.
                :param n: the node itself
                :param name: the name of the child to remove
                :type n: Sofa.Simulation.Node&
                :type name: string
                :Examples:
                >>> node1.removeChild("nameNode2")
                
        """
    @typing.overload
    def removeChild(self, arg0: str) -> object: ...
    def removeObject(self, arg0: Object) -> None: 
        """
                Remove an object
                :param object: the object to be removed
                :type object: BaseObject
                
        """
    def sendEvent(self, arg0: object, arg1: str) -> None: 
        """
                Send an event to other nodes,
                by creating a PythonScriptEvent and propagating it to the rest of the tree.
                Only the nodes and objects downstream will receive the message.
                :param pyUserData: the user data that can be sent
                :param eventName: the name of the event
                :type pyUserData: py::object
                :type eventName: string
                
        """
    @property
    def children(self) -> sofapython3::NodeIterator:
        """
                Field interface to acces the children of a node.

                :Example:
                >>> n = Sofa.Core.Node("MyNode")
                >>> for child in n.children:
                >>>     print(child.name)
                

        :type: sofapython3::NodeIterator
        """
    @property
    def objects(self) -> sofapython3::NodeIterator:
        """
                Field interface to acces the objects of a node.

                :Example:
                >>> n = Sofa.Core.Node("MyNode")
                >>> for object in n.objects:
                >>>     print(object.name)
                

        :type: sofapython3::NodeIterator
        """
    @property
    def parents(self) -> sofapython3::NodeIterator:
        """
                Field interface to acces the parents of a node.
                :Example:
                >>> n = Sofa.Core.Node("MyNode")
                >>> for parent in n.parents:
                >>>     print(parent.name)
                

        :type: sofapython3::NodeIterator
        """
    pass
class Controller(Object, Base):
    """
            An Interface for user interaction on SOFA Components.

            It can catch events to trigger actions, such as onAnimateBeginEvent, onAnimateEndEvent or onPythonScriptEvent.
            A new custom controller class needs to be defined to use a controller in a script,
            and that class needs to reimplement the __init__ method.

            :example of use:

            In the following example, we redefine the controller class, and reimplement the __init__ method.
            We also implement the onAnimateBeginEvent, that will be activted everytime an animation step ends
            and that will simply print a message in the command line. In the createScene function, we initialize
            the controller and add it to the rootNode.
            If you run this with runSofa, it will simply endlessly print `onAnimateBeginEvent`
            when you click the Animate button.

                .. code-block:: python

                    import Sofa.Core

                    class MyController(Sofa.Core.Controller):
                        def __init__(self, *args, **kwargs):
                             ## These are needed (and the normal way to override from a python class)
                             Sofa.Core.Controller.__init__(self, *args, **kwargs)
                             print(" Python::__init__::"+str(self.name))

                        def onEvent(self, event):
                             """This function is the fallback one that is called if the XXXX event is
                                received but there is not overriden onXXXX() method.
                             """
                             print("generic event handler catched ", event)

                        def onAnimateBeginEvent(self, event):
                             print("onAnimateBeginEvent")

                    def createScene(rootNode):
                        controller = MyController(name="MyC")
                        rootNode.addObject(controller)
                        return rootNode
             
    """
    def __init__(self, *args, **kwargs) -> None: ...
    def init(self) -> None: ...
    def reinit(self) -> None: ...
    pass
class Data():
    """
            The purpose of a data is to store the attributs of your component.

            :Create a data field:

            To create a data field, you can use the function addData, as shown in the example below. Note that the help
            parameter allows you  to create a help message for the data field, accessible with the getHelp method.

            .. code-block:: python

                import Sofa.Core

                root = Sofa.Core.Node("root")
                root.addData(name="aField", value=1.0 , help="help message",group="theDataGroup",
                    type="float")

            :Access the value of a data field in read-only:

            Accessing a data field is easy thanks to the generalized access API. You simply need the name of the data field,
            as shown in the following :

            .. code-block:: python

                import Sofa.Core

                root = Sofa.Core.Node("root")
                root.addData(name="aField", value=1.0 , help="help message",group="theDataGroup",
                    type="float")
                valueOfaField = root.aField.value
                print(valueOfaField) #will print '1.0'

            Please note that here valueOfaField is a copy of the real value of aField. You can change its value, but it won't change the value of aField.

            SOFA elements are often created with data fields, so you won't always need to create data fields.
            For example, most SOFA elements are created with the data field `name` :

            .. code-block:: python

                import Sofa.Core

                root = Sofa.Core.Node("rootNode")
                print(root.name.value) #will print 'rootNode'

            However, this API only allows you to access the value of a data field, but not to change it.

            :Change the value of a data field:

            To change the value of a data field, there are two possibilities. If the type of the data field is a scalar, you can do the following :

            .. code-block:: python

                import Sofa.Core

                root = Sofa.Core.Node("root")
                root.addData(name="aField", value=1.0 , help="help message",group="theDataGroup",
                    type="float")
                root.aField = 3.0 # will change the value of aField
                print(root.aField.value) # will print '3.0'

            Or you can create a temporary pointer that will allow you to change the value of aField, instead of giving it a whole new one,
            which is more cost effective when the type of the data field is something other than a scalar:

            .. code-block:: python

                import Sofa.Core

                root = Sofa.Core.Node("root")
                root.addData(name="aField", value=[0.0, 1.0, 3.0] , help="help message",group="theDataGroup",
                    type="Vec3d")
                with root.aField.writeable() as value:
                        value[0] = 3.0

                print(root.aField.value) # will print '[3.0, 1.0, 3.0]'

            
    """
    def __getattr__(self, arg0: str) -> object: ...
    def __repr__(self) -> str: ...
    def __setattr__(self, arg0: str, arg1: object) -> None: ...
    def __str__(self) -> str: ...
    def array(self) -> numpy.ndarray: 
        """
                Return a python read-only array of the data.
                :rtype: read-only array
                
        """
    def getCounter(self) -> int: 
        """
                Return the number of changes since creation.
                This can be used to efficiently detect changes.
                :rtype: integer
                
        """
    def getHelp(self) -> str: 
        """
                Get the help message.
                :rtype: string
                
        """
    def getLinkPath(self) -> str: 
        """
                Return the path of the link to the data.
                :rtype: string
                
        """
    def getName(self) -> str: 
        """
                Get the name of the data.
                :rtype: string
                
        """
    def getOwner(self) -> object: 
        """
                Return the Base component owning this data.
                :rtype: Base
                
        """
    def getParent(self) -> Data: 
        """
                Return the parent data of this data.
                :rtype: BaseData
                
        """
    def getPathName(self) -> str: 
        """
                Return the name of the path of the data.
                :rtype: string
                
        """
    def getValueString(self) -> str: 
        """
                Print the value of the associated variable.
                
        """
    def getValueTypeString(self) -> str: 
        """
                Print the value of the associated variable.
                
        """
    def getValueVoidPtr(self) -> capsule: 
        """
                Get a constant void pointer to the value held in this data.
                
        """
    def hasChanged(self) -> bool: 
        """
                Update the data if it is dirty.
                :return: true if it is updated, false otherwise.
                ;rtype: boolean
                
        """
    def hasParent(self) -> bool: 
        """
                Check whether the data has a parent.
                :return: true if the data has a parent.
                :rtype: boolean
                
        """
    def isDirty(self) -> bool: 
        """
                Return true if the DDGNode needs to be updated.
                
        """
    def isPersistent(self) -> bool: 
        """
                Alias to match BaseData API.
                Checks whether the link requires a path in order to be created.
                :return: true if the link requires a path.
                :rtype: boolean
                
        """
    def isReadOnly(self) -> bool: 
        """
                Return whether this data will be read-only in GUIs.
                :return: True for read-only
                
        """
    def isRequired(self) -> bool: 
        """
                Check whether a data is flagged as required for the creation of an object.
                :return: true if it is required.
                
        """
    def isSet(self) -> bool: 
        """
                Check if the value has been modified
                If this data is linked, the value of this data will be considered as modified
                (even if the parent's value has not been modified)
                :return: True if the data has been modified
                
        """
    def read(self, arg0: str) -> bool: 
        """
                Read the command line, set the value of the data to the given content of the command line.
                :param str: the content of the command line
                :type str: string
                
        """
    def setName(self, arg0: str) -> None: 
        """
                Set a new name for the data.
                :param name: the new name
                :type name: string
                
        """
    def setParent(self, arg0: Data) -> None: 
        """
                Set a new parent and a new path for the data.
                :param parent: the new parent of the data
                :param path: the new path of the data
                :type parent: BaseData
                :type path: string
                
        """
    def setPersistent(self, arg0: bool) -> None: 
        """
                Alias to match BaseData API.
                Changes the value of a flag,
                that should be set to true if link requires a path string in order to be created.
                :param bool: new value for the flag.
                :type bool: boolean
                
        """
    def setReadOnly(self, arg0: bool) -> None: 
        """
                Set whether this data is read-only. (True for read-only)
                :param bool: value of the read-only property
                :type bool: boolean
                
        """
    def toList(self) -> list: 
        """
                Return a list containing the value of the vector of data.
                
        """
    def typeName(self) -> str: 
        """
                Return the name of the type of the data.
                :rtype: string
                
        """
    def unset(self) -> None: 
        """
                Reset the isSet flag to false, to indicate that the current value is the default for this data.
                
        """
    def updateIfDirty(self) -> None: 
        """
                Utility method to call update if necessary.
                This method should be called before reading or writing the value of this node.
                
        """
    @typing.overload
    def writeableArray(self) -> object: 
        """
                Return a writable array of the data given an Object.
                :param f: the object to convert into an array.
                :type f: python object
                :rtype: writeable array
                


                Return a new empty array.
                :rtype: writeable array
                
        """
    @typing.overload
    def writeableArray(self, arg0: object) -> object: ...
    pass
class DataContainer(Data):
    """
            Classic python container implemented for SOFA data fields.

            
    """
    def __add__(self, arg0: object) -> object: ...
    def __getitem__(self, arg0: object) -> object: ...
    def __iadd__(self, arg0: object) -> DataContainer: ...
    def __imul__(self, arg0: object) -> object: ...
    def __isub__(self, arg0: object) -> DataContainer: ...
    def __len__(self) -> int: 
        """
                Return the number of elements in the container's 1st
                dimension. (equivalent to shape(0))
                
        """
    def __mul__(self, arg0: object) -> object: ...
    def __repr__(self) -> str: ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: object) -> None: ...
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: object) -> object: ...
    @typing.overload
    def __setitem__(self, arg0: tuple, arg1: object) -> None: ...
    def __str__(self) -> str: ...
    def __sub__(self, arg0: object) -> object: ...
    def apply(self, arg0: function) -> None: ...
    def array(self) -> numpy.ndarray: ...
    def toList(self) -> object: ...
    @typing.overload
    def writeable(self) -> object: ...
    @typing.overload
    def writeable(self, arg0: object) -> object: ...
    @property
    def ndim(self) -> int:
        """
                Return the number of dimensions in the container
                

        :type: int
        """
    @property
    def shape(self) -> tuple:
        """
                Return the shape of the container (a tuple of length ndim,
                with each value being equal to the length of the given dimension)
                

        :type: tuple
        """
    @property
    def size(self) -> int:
        """
                Return the total number of elements in the container
                ( equivalent to math.prod(self.shape()) )
                

        :type: int
        """
    pass
class DataContainerContextManager():
    def __enter__(self) -> object: ...
    def __exit__(self, arg0: object, arg1: object, arg2: object) -> None: ...
    pass
class DataDict():
    """
            DataDict exposes the data of a sofa object in a way similar to a normal python dictionnary.

            :Example:
                .. code-block:: python

                    for k,v in anObject.__data__.items():
                    print("Data name :"+k+" value:" +str(v)))
            
    """
    @typing.overload
    def __getitem__(self, arg0: int) -> object: ...
    @typing.overload
    def __getitem__(self, arg0: str) -> object: ...
    @staticmethod
    def __iter__(*args, **kwargs) -> typing.Any: ...
    def __len__(self) -> int: ...
    def __setitem__(self, arg0: str, arg1: object) -> float: ...
    @staticmethod
    def items(*args, **kwargs) -> typing.Any: 
        """
                Expose the data, both the key and the value of each item.
                :Example:
                    .. code-block:: python

                        for k,v in anObject.__data__.items():
                        print("Data name :"+k+" value:" +str(v)))
                
        """
    @staticmethod
    def keys(*args, **kwargs) -> typing.Any: 
        """
                Expose the data, but only the key (the name) of each items
                
        """
    @staticmethod
    def values(*args, **kwargs) -> typing.Any: 
        """
                Expose the data, but only the value of each items
                
        """
    pass
class DataDictIterator():
    """
            Iterator reimplemented to fit dataDict.

            
    """
    def __iter__(self) -> DataDictIterator: ...
    def __next__(self) -> object: ...
    pass
class DataEngine(Object, Base):
    """
            DataEngine is a class that computes a set of Data outputs from a set of Data inputs.

            It is a trampoline class that enables python implementations
            of the C++ sofa::core::DataEngine class
            
    """
    def __init__(self, *args, **kwargs) -> None: ...
    def addInput(self, arg0: Data) -> None: 
        """
                Registers a data as an input for the DataEngine.

                :param data: The data to register as input.
                :type data: Sofa.Core.Data.
                :returns:  None.
                
        """
    def addOutput(self, arg0: Data) -> None: 
        """
                Registers a data field as being an output for this engine.
                If an Engine's output is accessed (getValue, beginEdit...)
                AND one or more of its input is dirty, the engine's update()
                method will be called
                :param data: the data to register as an output
                :rtype: None
                
        """
    pass
class DataLink(Data):
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    pass
class DataString(Data):
    def __eq__(self, arg0: Data) -> bool_: ...
    def __getitem__(self, arg0: int) -> object: ...
    def __len__(self) -> int: ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    __hash__ = None
    pass
class DataVectorString(Data):
    def __getitem__(self, arg0: int) -> object: ...
    def __len__(self) -> int: ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    pass
class ForceFieldRigid2d(Object, Base):
    """
                             An overridable class to create your own customized force field.
                             
    """
    def __init__(self, *args, **kwargs) -> None: ...
    pass
class ForceFieldRigid3d(Object, Base):
    """
                             An overridable class to create your own customized force field.
                             
    """
    def __init__(self, *args, **kwargs) -> None: ...
    pass
class ForceFieldVec1d(Object, Base):
    """
                             An overridable class to create your own customized force field.
                             
    """
    def __init__(self, *args, **kwargs) -> None: ...
    pass
class ForceFieldVec2d(Object, Base):
    """
                             An overridable class to create your own customized force field.
                             
    """
    def __init__(self, *args, **kwargs) -> None: ...
    pass
class ForceFieldVec3d(Object, Base):
    """
                             An overridable class to create your own customized force field.
                             
    """
    def __init__(self, *args, **kwargs) -> None: ...
    pass
class Link():
    """
            A Link allows you to access a sofa component from another one anywhere in the simulation graph.
            In your scene creation file, it usually appear as : input=@../component.

            :Example of use:

            .. code-block:: python

                import Sofa.Core
                import SofaRuntime
                SofaRuntime.importPlugin("SofaComponentAll")

                root = Sofa.Core.Node("root")
                root.addObject("MechanicalObject", name="t")
                link = root.findLink("mechanicalState") # access the link
                link.getValueString() # will print '@/t'
                link.getHelp() # will print the help message for all mechanical states
            
    """
    def getHelp(self) -> str: 
        """
                Get the help message.
                :rtype: string
                
        """
    def getLinkedBase(self, index: int = 0) -> object: 
        """
                Get the linked base.
                :param index: index of the linked base.
                :type index: int
                
        """
    def getLinkedPath(self, index: int = 0) -> str: 
        """
                Get the linked path.
                :param index: index of the linked path.
                :type index: int
                
        """
    def getName(self) -> str: 
        """
                Get the name of the link.
                :rtype: string
                
        """
    def getOwnerBase(self) -> object: 
        """
                Get the base owner of the link.
                :rtype: Base
                
        """
    def getPathName(self) -> str: 
        """
                Get the linked path.
                :param index: index of the linked path.
                :type index: int
                
        """
    def getSize(self) -> int: 
        """
                Get the size of the link.
                :rtype: size_t
                
        """
    def getValueString(self) -> str: 
        """
                Print the value of the associated variable.
                
        """
    def getValueTypeString(self) -> str: 
        """
                Print the value of the associated variable.
                
        """
    def isMultiLink(self) -> bool: 
        """
                Checks whether the link is an array.
                :return: true if link is an array.
                :rtype: boolean
                
        """
    def isPersistent(self) -> bool: 
        """
                Alias to match BaseData API.
                Checks whether the link requires a path in order to be created.
                :return: true if the link requires a path.
                :rtype: boolean
                
        """
    def isReadOnly(self) -> bool: 
        """
                Alias to match BaseData API.
                Checks whether the link requires a path in order to be created.
                :return: true if the link doesn't require a path.
                :rtype: boolean
                
        """
    def read(self, arg0: str) -> bool: 
        """
                Read the command line.
                :param str: the content of the command line
                :type str: string
                
        """
    def setHelp(self, arg0: str) -> None: 
        """
                Set the help message.
                :param message: the new help message
                :type message: string
                
        """
    def setLinkedBase(self, arg0: Base) -> None: 
        """
                Get the linked base.
                :param index: index of the linked base.
                :type index: int
                
        """
    def setName(self, arg0: str) -> None: 
        """
                Set a new name for the link.
                :param name: the name to be given to the link
                :type name: string
                
        """
    def setPersistent(self, arg0: bool) -> None: 
        """
                Alias to match BaseData API.
                Changes the value of a flag,
                that should be set to true if link requires a path string in order to be created.
                :param bool: new value for the flag.
                :type bool: boolean
                
        """
    pass
class BasePrefab(Node, BaseNode, Context, BaseContext, Base):
    pass
class NodeIterator():
    @typing.overload
    def __getitem__(self, arg0: int) -> object: ...
    @typing.overload
    def __getitem__(self, arg0: str) -> object: ...
    def __iter__(self) -> NodeIterator: ...
    def __len__(self) -> object: ...
    def __next__(self) -> object: ...
    def at(self, arg0: int) -> object: ...
    def remove_at(self, arg0: int) -> None: ...
    pass
class Camera(Object, Base):
    """
            Camera is an API dedicated to the control of the Camera in SOFA.

            :Example of use:

            .. code-block:: python

                import Sofa.Core
                import SofaRuntime
                SofaRuntime.importPlugin("SofaComponentAll")

                root = Sofa.Core.Node("root")
                root.addObject("Camera", name="c")
                root.c.position.value # Access the position of the camera
                root.c.orientation.value # Access the orientation of the camera in quaternion

            :More complex example of use:

            The camera, added to a controller, can allow the user to create its own script to move and orientate the camera.

            In the example below, the mouse is used to orientate the camera, and the following commands are used to move it : ctrl+z to move forward,
            ctrl+x to move backward, ctrl+d to translate on the right, ctrl+q to translate on the left.
            The code for this example is available in the folder examples under the name BaseCamera.py of the SofaPython3 GitHub repository.

            .. image:: ../../../../../images/baseCamera.gif
                :alt: Example of a controller for a BaseCamera
                :align: center
                :height: 250pt

            
    """
    def Orthographic(self) -> None: ...
    def Perspective(self) -> None: ...
    @staticmethod
    def getLookAtFromOrientation(*args, **kwargs) -> typing.Any: ...
    def getModelViewMatrix(self) -> typing.List[float]: ...
    def getOpenGLModelViewMatrix(self) -> typing.List[float]: ...
    def getOpenGLProjectionMatrix(self) -> typing.List[float]: ...
    @staticmethod
    def getOrientationFromLookAt(*args, **kwargs) -> typing.Any: ...
    @staticmethod
    def getPositionFromOrientation(*args, **kwargs) -> typing.Any: ...
    def getProjectionMatrix(self) -> typing.List[float]: ...
    def rotate(self, arg0: list) -> None: ...
    def rotateCameraAroundPoint(self, arg0: list, arg1: list) -> None: ...
    def setCameraType(self, arg0: int) -> None: ...
    pass
class ObjectFactory():
    """
        Main class used to register and dynamically create objects.

        It uses the Factory design pattern, where each class is registered in a map,
        and dynamically retrieved given the type name.

        It also stores metainformation on each classes, such as description,
        authors, license, and available template types.
        
    """
    @staticmethod
    def getComponent(*args, **kwargs) -> typing.Any: 
        """
            Get an entry given a class name (or alias)
            
        """
    @staticmethod
    def getComponentsFromTarget(*args, **kwargs) -> typing.Any: 
        """
            Return the registered classes from a given target
            
        """
    @staticmethod
    def shortName(arg0: str) -> str: 
        """
            Return the shortname for this classname. Empty string if no creator exists for this classname.
            
        """
    components: list # value = [<AddDataRepository>, <AddPluginRepository>, <BackgroundSetting>, <BruteForceDetection>, <Camera>, <CapsuleCollisionModel>, <ContactListener>, <CubeCollisionModel>, <CylinderCollisionModel>, <DefaultAnimationLoop>, <DefaultContactManager>, <DefaultPipeline>, <DefaultVisualManagerLoop>, <DiscreteIntersection>, <EdgeSetGeometryAlgorithms>, <EdgeSetTopologyContainer>, <EdgeSetTopologyModifier>, <FileMessageHandlerComponent>, <GridTopology>, <HexahedronSetGeometryAlgorithms>, <HexahedronSetTopologyContainer>, <HexahedronSetTopologyModifier>, <InfoComponent>, <InteractiveCamera>, <MakeAlias>, <MakeDataAlias>, <MeshTopology>, <MessageHandlerComponent>, <MinProximityIntersection>, <NewProximityIntersection>, <OBBCollisionModel>, <PointSetGeometryAlgorithms>, <PointSetTopologyContainer>, <PointSetTopologyModifier>, <QuadSetGeometryAlgorithms>, <QuadSetTopologyContainer>, <QuadSetTopologyModifier>, <RegularGridTopology>, <RequiredPlugin>, <SparseGridTopology>, <SphereCollisionModel>, <TetrahedronSetGeometryAlgorithms>, <TetrahedronSetTopologyContainer>, <TetrahedronSetTopologyModifier>, <TriangleSetGeometryAlgorithms>, <TriangleSetTopologyContainer>, <TriangleSetTopologyModifier>, <VisualModelImpl>, <VisualStyle>]
    targets = {'SofaBaseVisual', 'SofaBaseUtils', 'SofaBaseTopology', 'SofaBaseCollision', 'SofaSimulationCore'}
    pass
class PrefabLink():
    @staticmethod
    @typing.overload
    def __init__(*args, **kwargs) -> typing.Any: ...
    @typing.overload
    def __init__(self, arg0: str) -> None: ...
    def getTargetBase(self) -> object: ...
    def getTargetPath(self) -> str: ...
    pass
class RawPrefab(BasePrefab, Node, BaseNode, Context, BaseContext, Base):
    """
             
    """
    def __init__(self, *args, **kwargs) -> None: ...
    def addPrefabParameter(self, name: str, help: str, type: str, default: object = None) -> None: ...
    def init(self) -> None: ...
    def reinit(self) -> None: ...
    def setSourceTracking(self, arg0: str) -> None: ...
    pass
class Topology(Base):
    pass
