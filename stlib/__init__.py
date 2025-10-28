__all__ = ["core","entities","geometries","materials","collision","visual"]

import Sofa.Core
from stlib.core.basePrefab import BasePrefab

def __genericAdd(self : Sofa.Core.Node, typeName, **kwargs):
    def findName(cname, names):
        """Compute a working unique name in the node"""
        rname = cname
        for i in range(0, len(names)):
            if rname not in names:
                return rname
            rname = cname + str(i+1)
        return rname


    def checkName(context : Sofa.Core.Node, name):
        # Check if the name already exists, if this happens, create a new one.
        if name in context.children or name in context.objects:
            names = {node.name.value for node in context.children}
            names = names.union({object.name.value for object in context.objects})
            name = findName(name, names)
        return name


    # Check if a name is provided, if not, use the one of the class
    params = kwargs.copy()
    if isinstance(typeName, type) and issubclass(typeName, BasePrefab): #Only for prefabs
        if "parameters" not in params or len(params.keys()) > 1:
            raise RuntimeError("Invalid argument, a prefab takes only the \"parameters\" kwargs as input")
    elif "name" not in params : #This doesn't apply to prefab
        if isinstance(typeName, str):
            params["name"] = typeName
        elif isinstance(typeName, type) and issubclass(typeName, Sofa.Core.Node):
            params["name"] = typeName.__name__
        elif isinstance(typeName, Sofa.Core.Node):
            params["name"] = "Node"
        elif isinstance(typeName, type) and issubclass(typeName, Sofa.Core.Object):
            params["name"] = typeName.name.value
        elif isinstance(typeName, type) and issubclass(typeName, Sofa.Core.ObjectDeclaration):
            params["name"] = typeName.__name__
        else:
            raise RuntimeError("Invalid argument ", typeName)

    if isinstance(typeName, type) and issubclass(typeName, BasePrefab):
        params["parameters"].name = checkName(self, params["parameters"].name)
    else:
        params["name"] = checkName(self, params["name"])

    # Dispatch the creation to either addObject or addChild
    if isinstance(typeName, type) and issubclass(typeName, BasePrefab):
        pref = self.addChild(typeName(**params))
        pref.init()
    elif isinstance(typeName, Sofa.Core.Node):
        pref = self.addChild(typeName(**params))
    elif isinstance(typeName, type) and issubclass(typeName, Sofa.Core.Object):
        pref = self.addObject(typeName(**params))
    elif isinstance(typeName, type) and issubclass(typeName, Sofa.Core.ObjectDeclaration):
        pref = self.addObject(typeName.__name__, **params)
    elif isinstance(typeName, str):
        pref = self.addObject(typeName, **params)
    else:
        raise RuntimeError("Invalid argument", typeName)
    return pref

# Inject the method so it become available as if it was part of Sofa.Core.Node
Sofa.Core.Node.add = __genericAdd
