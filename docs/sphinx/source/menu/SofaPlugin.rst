The SofaPython3 plugin
================================

The SofaPython3 plugins embeds a python interpreter into a Sofa simulation. 

Using SofaPyton3 in runSofa:
----------------------------

Using SofaPython3 in runSofa requires loading the SofaPython3 plugin in your runSofa environment.

You can do this by loading the plugin using the PluginManager (in the GUI) or by auto-loading the plugin in runSofa: simply copy the file <SOFA_build>/lib/plugin_list.conf.default, and rename it plugin_list.conf, then add the line:

.. code-block:: bash

	SofaPython3 NO_VERSION

Adding the line to the file <SOFA_build>/lib/plugin_list.conf.default will work, but you will need to add the line everytime you compile the code.
Having the SofaPython3 plugin active will allow you to open scene files using the ".py, .py3, .pyscn, .pyscn3" file extension in runSofa, with the command :

.. code-block:: bash

	<SOFA_build>/bin/runSofa <your_file>

Using SofaPython3 from the command line:
----------------------------------------

It is possible to use Sofa in a Python3 command line, as long as the right modules are imported. The following code should cover most basic SOFA elements:

.. code-block:: python

        # to be able to create sofa objects you need to first load the plugins that implement them.
        # For simplicity you can load the plugin "SofaAllCommonComponents" that will load all most
        # common sofa object.
        import SofaRuntime
        SofaRuntime.importPlugin("SofaAllCommonComponents")

        # to create elements like Node or objects
	import Sofa.Core


Create your first SOFA scene using the SofaPython3 plugin:
----------------------------------------------------------

A scene in SOFA is an ordered tree of nodes representing objects (example of node: hand), with parent/child relationship (example of hand's child: finger). Each node has one or more components. Every node and component has a name and a few features. The main node at the top of the tree is usually called "rootNode" or "root".

Just like with the legacy SofaPython plugin, SofaPython3 searches for a `createScene(arg0: Sofa.Core.Node) -> Sofa.Core.Node` method in the python script, to use as the entry point of the SOFA simulation, and taking a single parameter: the root Node.  Thus define this method:

.. code-block:: python
		
	def createScene(rootNode):
		#Doesn't do anything yet
		return rootNode

You can then load a python scene in Sofa, but it doesn't do much. Let's enrich this scene!

Create your first element
^^^^^^^^^^^^^^^^^^^^^^^^^
First things first, we import the module Sofa.Core, to have access to the functions we will need.
Then, we add a grid, in order to see things more clearly. To do that, we simply need to add an object to the rootNode with the right properties :

.. code-block:: python 

	import Sofa.Core

	def createScene(rootNode):
		rootNode.addObject("OglGrid", nbSubdiv=10, size=1000)

Now, we create a new child node, in order to add the general configuration of the scene : required plugins (here SofaPython3) and other tools (like a system of axes).

.. code-block:: python

	confignode = rootNode.addChild("Config")
	confignode.addObject('RequiredPlugin', name="SofaPython3", printLog=False)
	confignode.addObject('OglSceneFrame', style="Arrows", alignment="TopRight")


Finally, we add the sphere itself, which consists of two parts : the mechanical representation and the visual representation of the sphere: 

.. code-block:: python

	#Creating the sphere
	sphere = rootNode.addChild("sphere")
	sphere.addObject('MechanicalObject', name="mstate", template="Rigid3",
	       translation2=[0., 0., 0.], rotation2=[0., 0., 0.], showObjectScale=50)

    	#### visualization
	sphereVisu = sphere.addChild("VisualModel")
	sphereVisu.loader = sphereVisu.addObject('MeshObjLoader', name="loader",
	       filename="mesh/ball.obj")
	sphereVisu.addObject('OglModel', name="model", src="@loader", scale3d=[50]*3,
	       color=[0., 1., 0.], updateNormals=False)
	sphereVisu.addObject('RigidMapping')

Now, if you execute your scene, you can see a sphere, but it won't move if you click on the Animate button in SOFA. Let's change that!

Add movements and forces
^^^^^^^^^^^^^^^^^^^^^^^^

A default gravity force is implemented on Sofa. Here we reset it, for learning purposes. We also define the time step of the simulation.

.. code-block:: python
	
	rootNode.findData('gravity').value=[0.0,-9.81,0.0];
	rootNode.findData('dt').value=0.01

We add a mechanical model, so that all our futur elements will have the same total mass, volume and inertia matrix :

.. code-block:: python

	totalMass = 1.0
	volume = 1.0
	inertiaMatrix=[1., 0., 0., 0., 1., 0., 0., 0., 1.]

We add properties to the sphere. First, we add a mass, then an object called 'UncoupledConstraintCorrection', in charge of computing the constraint forces of the sphere, then we add two different solvers. One is a time integration scheme that defines the system to be solved at each time step of the simulation (here the implicit Euler Method), the other is a solving method (here the Conjugate Gradient method), that solves the equations governing the model at each time step, and updates the MechanicalObject.

.. code-block:: python

	sphere.addObject('UniformMass', name="mass", vertexMass=[totalMass, volume, inertiaMatrix[:]])
	sphere.addObject('UncoupledConstraintCorrection')

	sphere.addObject('EulerImplicitSolver', name='odesolver')
	sphere.addObject('CGLinearSolver', name='Solver')

Now, if you click on the Animate button in SOFA, the sphere will fall.

Add a second element 
^^^^^^^^^^^^^^^^^^^^

Let's add a second element, a floor, to see how they interact :

.. code-block:: python

        floor = rootNode.addChild("floor")
	floor.addObject('MechanicalObject', name="mstate", template="Rigid3", 
		translation2=[0.0,-300.0,0.0], rotation2=[0., 0., 0.], showObjectScale=5.0)
	floor.addObject('UniformMass', name="mass", vertexMass=[totalMass, volume, inertiaMatrix[:]])

	floorVisu = floor.addChild("VisualModel")
	floorVisu.loader = floorVisu.addObject('MeshObjLoader', name="loader", filename="mesh/floor.obj")
	floorVisu.addObject('OglModel', name="model", src="@loader", scale3d=[5.0]*3, color=[1., 1., 0.], 
		updateNormals=False)
	floorVisu.addObject('RigidMapping')
        
A floor has now been added to the scene. It is a stationnary object, it won't move during the simulation. When you click on the Animate button, you can see that the sphere goes through the floor, as if there were nothing there. That is because there is no collision modeling in the scene yet.

Add a collision model
^^^^^^^^^^^^^^^^^^^^^

We first add a collision model for the scene in general, that is stating how a contact between the objects is handled: here the objects must not be able to go through one another. Potential collisions are looked for within an alarmDistance radius from the objet. If a collision situation is detected, the collision model computes the behaviour of the objects, which are stopped at a ContactDistance from each other.

.. code-block:: python

	rootNode.addObject('DefaultPipeline')
	rootNode.addObject('FreeMotionAnimationLoop')
	rootNode.addObject('GenericConstraintSolver', tolerance="1e-6", maxIterations="1000")
	rootNode.addObject('BruteForceDetection')
	rootNode.addObject('RuleBasedContactManager', responseParams="mu="+str(0.0), 
		name='Response', response='FrictionContact')
	rootNode.addObject('LocalMinDistance', alarmDistance=10, contactDistance=5, angleCone=0.01)

We add a new child node to the sphere, that will be in charge of processing the collision.

.. code-block:: python

	collision = sphere.addChild('collision')
	collision.addObject('MeshObjLoader', name="loader", filename="mesh/ball.obj",
		triangulate="true", scale=45.0)

	collision.addObject('MeshTopology', src="@loader")
	collision.addObject('MechanicalObject')

	collision.addObject('TriangleCollisionModel')
	collision.addObject('LineCollisionModel')
	collision.addObject('PointCollisionModel')

	collision.addObject('RigidMapping')

We do the same for the floor, but we also specify that the floor is a stationnary object that shouldn't move.

.. code-block:: python

	floorCollis = floor.addChild('collision')
	floorCollis.addObject('MeshObjLoader', name="loader", filename="mesh/floor.obj",
		triangulate="true", scale=5.0)
	floorCollis.addObject('MeshTopology', src="@loader")
	floorCollis.addObject('MechanicalObject')
	floorCollis.addObject('TriangleCollisionModel', moving=False, simulated=False)
	floorCollis.addObject('LineCollisionModel', moving=False, simulated=False)
	floorCollis.addObject('PointCollisionModel', moving=False, simulated=False)

	floorCollis.addObject('RigidMapping')

Now, the sphere is stopped by the floor, as it should be.
Congratulations! You made your first SOFA scene in Python3!

For more information on how to use the SOFA modules bindings in python, visit this page: :doc:`SofaModule`

Here is the entire code of the scene :

.. code-block:: python

	import Sofa.Core
	def createScene(rootNode):
		rootNode.addObject("OglGrid", nbSubdiv=10, size=1000)

		rootNode.findData('gravity').value=[0.0,-981.0,0.0];
		rootNode.findData('dt').value=0.01

		confignode = rootNode.addChild("Config")
		confignode.addObject('RequiredPlugin', name="SofaMiscCollision", printLog=False)
		confignode.addObject('RequiredPlugin', name="SofaPython3", printLog=False)
		confignode.addObject('OglSceneFrame', style="Arrows", alignment="TopRight")


	 	#Collision function

		rootNode.addObject('DefaultPipeline')
		rootNode.addObject('FreeMotionAnimationLoop')
		rootNode.addObject('GenericConstraintSolver', tolerance="1e-6", maxIterations="1000")
		rootNode.addObject('BruteForceDetection')
		rootNode.addObject('RuleBasedContactManager', responseParams="mu="+str(0.0), name='Response',
			response='FrictionContact')
		rootNode.addObject('LocalMinDistance', alarmDistance=10, contactDistance=5, angleCone=0.01)

		### Mechanical model

		totalMass = 1.0
		volume = 1.0
		inertiaMatrix=[1., 0., 0., 0., 1., 0., 0., 0., 1.]

		#Creating the floor
		floor = rootNode.addChild("floor")

		floor.addObject('MechanicalObject', name="mstate", template="Rigid3", 
			translation2=[0.0,-300.0,0.0], rotation2=[0., 0., 0.], showObjectScale=5.0)

		floor.addObject('UniformMass', name="mass", vertexMass=[totalMass, volume, inertiaMatrix[:]])
		floorCollis = floor.addChild('collision')
		floorCollis.addObject('MeshObjLoader', name="loader", filename="mesh/floor.obj",
			triangulate="true", scale=5.0)
		floorCollis.addObject('MeshTopology', src="@loader")
		floorCollis.addObject('MechanicalObject')
		floorCollis.addObject('TriangleCollisionModel', moving=False, simulated=False)
		floorCollis.addObject('LineCollisionModel', moving=False, simulated=False)
		floorCollis.addObject('PointCollisionModel', moving=False, simulated=False)

		floorCollis.addObject('RigidMapping')

	    	#### visualization
		floorVisu = floor.addChild("VisualModel")
		floorVisu.loader = floorVisu.addObject('MeshObjLoader', name="loader", 
			filename="mesh/floor.obj")
		floorVisu.addObject('OglModel', name="model", src="@loader", scale3d=[5.0]*3, 
			color=[1., 1., 0.], updateNormals=False)
		floorVisu.addObject('RigidMapping')

		#Creating the sphere
		sphere = rootNode.addChild("sphere")
		sphere.addObject('MechanicalObject', name="mstate", template="Rigid3",
			      translation2=[0., 0., 0.], rotation2=[0., 0., 0.], showObjectScale=50)
		sphere.addObject('UniformMass', name="mass", vertexMass=[totalMass, volume, inertiaMatrix[:]])
		sphere.addObject('UncoupledConstraintCorrection')

		sphere.addObject('EulerImplicitSolver', name='odesolver')
		sphere.addObject('CGLinearSolver', name='Solver')

		collision = sphere.addChild('collision')
		collision.addObject('MeshObjLoader', name="loader", filename="mesh/ball.obj",
			triangulate="true", scale=45.0)

		collision.addObject('MeshTopology', src="@loader")
		collision.addObject('MechanicalObject')

		collision.addObject('TriangleCollisionModel')
		collision.addObject('LineCollisionModel')
		collision.addObject('PointCollisionModel')

		collision.addObject('RigidMapping')

	    	#### visualization
		sphereVisu = sphere.addChild("VisualModel")
		sphereVisu.loader = sphereVisu.addObject('MeshObjLoader', name="loader",
			filename="mesh/ball.obj")
		sphereVisu.addObject('OglModel', name="model", src="@loader", scale3d=[50]*3, 
			color=[0., 1., 0.], updateNormals=False)
		sphereVisu.addObject('RigidMapping')

		return rootNode


