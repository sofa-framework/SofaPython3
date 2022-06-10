import Sofa

class MyPrefab(Sofa.Prefab):
    """
    A CustomPrefab that creates a given number of child node.
    """

    # These prefabParameters are special, when any of them are changed the prefab is completely recreated by calling the method onParameterChanged(). 
    # The scene graph is always kept synchronized with these parameters content.    
    prefabParameters = [
        {"name" : "myParameter", "type" : "int", "help" : "my message to help the user", "default" : 0}
    ]  

    # These prefabData are registering sofa data field to the prefab. 
    prefabData = [
        {"name" : "myData", "type" : "Vec3d", "help" : "my message to help the user", "default" : [0.0,0.0,0.0]}
    ]  

    # It is not possible to add properties in the __init__constructor. 
    initCount = 0

    def __init__(self, *args, **kwargs):
        Sofa.Prefab.__init__(self, *args, **kwargs)

    def init(self):
        """This method is called to fill the prefab's content."""
        for i in range(self.myParameter.value):
            self.addChild("Child"+str(i))
        self.addObject("MechanicalObject", position=self.myData.getLinkPath(), showObject=True, showObjectScale=2)            
        self.initCount += 1

    # By default, the onParameterChanged method is calling self.init()
    # Override this method to control when the update is done
    #def onParameterChanged(self):
    #    if self.initCount <= 4:
    #        print("Update the prefab")
    #        self.init()

def createScene(root):
    root.addChild(MyPrefab(name="MyPrefab1", myParameter=1, myData=[1.0,2.0,3.0]))
    root.addChild(MyPrefab(name="MyPrefab2", myParameter=5))
