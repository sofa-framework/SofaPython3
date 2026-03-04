import copy
import Sofa 
import Sofa.Core 
from stlib.core.basePrefabParameters import BasePrefabParameters

class BasePrefab(Sofa.Core.Node):
    """
    A Prefab is a Sofa.Node that assembles a set of components and nodes
    """

    def __init__(self, parameters: BasePrefabParameters):
        Sofa.Core.Node.__init__(self, name=parameters.name)
        self.parameters = parameters

    def init(self):
        raise NotImplemented("To be overridden by child class")
        
    def applyModifier(self, modifierType, parameters, *args ):
        if "Modifiers" not in self.children:
            self.addChild("Modifiers")

        modifier = self.Modifiers.addObject(modifierType(parameters=parameters))
        modifier.modify(*args)



    def localToGlobalCoordinates(pointCloudInput, pointCloudOutput):
        raise NotImplemented("Send an email to Damien, he will help you. Guaranteed :)")
    

        
