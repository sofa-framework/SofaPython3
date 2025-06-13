__all__ = ["core","entities","prefabs","shapes"]

import Sofa.Core
def __genericAdd(self : Sofa.Core.Node, typeName, **kwargs):
    def findName(cname, names):
        """Compute a working unique name in the node"""
        rname = cname
        for i in range(0, len(names)):
            if rname not in names:
                return rname
            rname = cname + str(i+1)
        return rname

    # Check if a name is provided, if not, use the one of the class
    params = kwargs.copy()
    isNode = False
    if "name" not in params:
        if isinstance(typeName, str):
            params["name"] = typeName
            isNode=True
        elif isinstance(typeName, type) and issubclass(typeName, Sofa.Core.Node):
            params["name"] = "Node"
            isNode=True
        elif isinstance(typeName, Sofa.Core.Node):
            params["name"] = "Node"
            isNode=True
        elif isinstance(typeName, type) and issubclass(typeName, Sofa.Core.Object):
            params["name"] = typeName.name.value
        elif  isinstance(typeName, type) and issubclass(typeName, Sofa.Core.ObjectDeclaration):
            params["name"] = typeName.__name__
        else:
            raise RuntimeError("Invalid argument ", typeName)

    # Check if the name already exists, if this happens, create a new one.
    if params["name"] in self.children or params["name"] in self.objects:
        names = {node.name.value for node in self.children}
        names = names.union({object.name.value for object in self.objects})
        params["name"] = findName(params["name"], names)

    # Dispatch the creation to either addObject or addChild
    if isinstance(typeName, type) and issubclass(typeName, Sofa.Core.Node):
        pref = self.addChild(typeName(params["name"]))
        pref.init()
    elif isinstance(typeName, Sofa.Core.Node):
        pref = self.addChild(typeName)
        pref.init()
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
