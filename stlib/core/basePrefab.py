import copy
import Sofa 
import Sofa.Core 
from stlib.core.basePrefabParameters import BasePrefabParameters

class BasePrefab(Sofa.Core.Node):
    """
    A Prefab is a Sofa.Node that assembles a set of components and nodes
    """

    def __init__(self, params: BasePrefabParameters):
        Sofa.Core.Node.__init__(self, name=params.name)
        self.parameters = params

    def init(self):
        raise NotImplemented("To be overridden by child class")
        
    
    def localToGlobalCoordinates(pointCloudInput, pointCloudOutput):
        raise NotImplemented("Send an email to Damien, he will help you. Guaranteed :)")
    

        
