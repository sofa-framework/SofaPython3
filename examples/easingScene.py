import Sofa.Core
import Sofa.splib.animation
from Sofa.splib.animation import easing
functionDict = {
	"sphere0":easing.sineIn, "sphere1":easing.sineOut, "sphere2":easing.sineInOut,
	"sphere3":easing.quadIn, "sphere4":easing.quadOut, "sphere5":easing.quadInOut,
	"sphere6":easing.cubicIn, "sphere7":easing.cubicOut, "sphere8":easing.cubicInOut,
	"sphere9":easing.quarticIn, "sphere10":easing.quarticOut, "sphere11":easing.quarticInOut,
	"sphere12":easing.quinticIn, "sphere13":easing.quinticOut, "sphere14":easing.quinticInOut,
	"sphere15":easing.expoIn, "sphere16":easing.expoOut, "sphere17":easing.expoInOut,
	"sphere18":easing.circIn, "sphere19":easing.circOut, "sphere20":easing.circInOut,
	"sphere21":easing.LinearRamp
}

def myAnimate(target, factor):
	with target.mstate.position.writeable() as tr:
		tr[0][0] = -20 + functionDict[target.name.value](factor)*25
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
	# This scene is an example scene that shows the different easing functions implemented in SofaPython3
	confignode = rootNode.addChild("Config")
	confignode.addObject('RequiredPlugin', name="SofaPython3", printLog=False)

	manager = Sofa.splib.animation.AnimationManager(rootNode)
	spheres = []
	for i in range(22):
		sphere = Sphere(rootNode, "sphere"+str(i), [-20,i,0,0,0,0,1],[i/10.0,i*0.7/10.0,0.9])
		spheres.append(sphere)
		Sofa.splib.animation.addAnimation(myAnimate, {"target": sphere}, 10.0, mode="pingpong")
	Sofa.Simulation.init(rootNode)
	print(len(spheres))
	return rootNode
