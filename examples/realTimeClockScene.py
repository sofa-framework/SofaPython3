import Sofa.Core
from splib.animation import AnimationManager, addAnimation
import Sofa.Core
from splib.animation import easing
from splib.animation import AnimationManager, addAnimation

def myAnimate(target, factor):
	with target.mstate.position.writeable() as tr:
		tr[0][0] = -20 + easing.sineInOut(factor)*20
	target.mstate.init()

def Sphere(rootNode, name, position, color):
	#Creating the sphere
	sphere = rootNode.addChild(name)
	sphere.addObject('MechanicalObject', name="mstate", template="Rigid3", position=position)
    	#### Visualization of the sphere
	sphereVisu = sphere.addChild("VisualModel")
	sphereVisu.loader = sphereVisu.addObject('MeshObjLoader', name="loader", filename="mesh/ball.obj", scale=0.5)
	sphereVisu.addObject('OglModel', name="model", src="@loader", color=color)
	sphereVisu.addObject('RigidMapping')
	return sphere

def createScene(rootNode):
	confignode = rootNode.addChild("Config")
	confignode.addObject('RequiredPlugin', name="SofaPython3", printLog=False)
	manager = AnimationManager(rootNode)
	rootNode.addObject(manager)
	sphere1 = Sphere(rootNode, "sphere1", [-20,0,0,0,0,0,1],[1/10.0,1*0.7/10.0,0.9])
	sphere2 = Sphere(rootNode, "sphere2", [-20,1,0,0,0,0,1],[7/10.0,7*0.7/10.0,0.9])
	addAnimation(myAnimate, {"target": sphere1}, 5, mode="pingpong")
	addAnimation(myAnimate, {"target": sphere2}, 5, mode="pingpong", realTimeClock=True)
	return rootNode
 
