import Sofa 
import Sofa.Core 
from stlib.core.basePrefabParameters import BasePrefabParameters

class BasePrefab(Sofa.Core.Node):

    parameters : BasePrefabParameters

    def __init__(self, parameters: BasePrefabParameters):
        Sofa.Core.Node.__init__(self, name=parameters.name)
        self.parameters = parameters

    def init(self):
        raise NotImplemented("This method should be implemented in the child class to initialize the prefab's components and nodes based on the provided parameters.")
        
        
