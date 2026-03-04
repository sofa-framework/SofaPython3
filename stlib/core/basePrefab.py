import Sofa 
import Sofa.Core 
from stlib.core.basePrefabParameters import BasePrefabParameters

class BasePrefab(Sofa.Core.Node):
    """
    A Prefab is a Sofa.Core.Node that assembles a set of components and nodes
    """

    parameters : BasePrefabParameters

    def __init__(self, parameters: BasePrefabParameters):
        Sofa.Core.Node.__init__(self, name=parameters.name)
        self.parameters = parameters

    def init(self):
        raise NotImplemented("To be overridden by child class")
        

        
