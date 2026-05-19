import Sofa 
import Sofa.Core 

from stlib.core.baseParameters import BaseParameters


class BasePrefabParameters(BaseParameters): 
    name : str = "object"
    kwargs : dict = {}

    # Transformation information
    # TODO: these data are going to be added in Node in SOFA (C++ implementation)
    translation : list[float] = [0., 0., 0.]
    rotation : list[float] = [0., 0., 0.]
    scale : list[float] = [1., 1., 1.]
        

class BasePrefab(Sofa.Core.Node):

    parameters : BasePrefabParameters

    def __init__(self, parameters: BasePrefabParameters):
        Sofa.Core.Node.__init__(self, name=parameters.name)
        self.parameters = parameters

    def init(self):
        raise NotImplemented("This method should be implemented in the child class to initialize the prefab's components and nodes based on the provided parameters.")
        
        
