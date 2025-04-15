import copy
import Sofa 
import Sofa.Core 
from stlib.core.basePrefabParameters import BasePrefabParameters


def addFromTypeName(self : Sofa.Core.Node, typeName, params = BasePrefabParameters, **kwargs):  
    def findName(cname, node):
        """Compute a working unique name in the node"""
        rname = cname 
        for i in range(0, len(node.children)):
            if rname not in node.children:
                return rname
            rname = cname + str(i+1)
        return rname

    for k,v in kwargs.items():
        if hasattr(params, k):
            setattr(params, k, v)

    params = copy.copy(params)
    if params.name in self.children:
        params.name = findName(params.name, self)

    return self.addChild(typeName(params)) 

Sofa.Core.Node.add = addFromTypeName


class BasePrefab(Sofa.Core.Node):
    """
    A Prefab is a Sofa.Node that assembles a set of components and nodes
    """

    def __init__(self, params: BasePrefabParameters):
        Sofa.Core.Node.__init__(self, name=params.name)

    def localToGlobalCoordinates(pointCloudInput, pointCloudOutput):
        raise NotImplemented("Send an email to Damien, he will help you. Guaranteed :)")
        
