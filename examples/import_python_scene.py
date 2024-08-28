import Sofa


def createScene(root):
	"""Demonstrates the use of Sofa.import_sofa_python_scene"""

	# loads the python modules "liver.py"
	liver = Sofa.import_sofa_python_scene("liver.py")

	# Creates the scene and attach it to A 
	a = root.addChild("A")
	liver.createScene(a)
	
	# Feel free to add any other objects to the scene (eg: controllers manipulating something in the liver) 
	...
	
