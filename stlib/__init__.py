__all__ = ["core","entities","prefabs","shapes"]

import Sofa.Core

from stlib.core.baseParameters import BaseParameters
from stlib.core.basePrefab import BasePrefab


def __genericAdd(self : Sofa.Core.Node, typeName, *param, **kwargs):


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


    if len(param) == 1 and isinstance(typeName, type) and not issubclass(typeName, BasePrefab):
        raise RuntimeError("Invalid argument : only prefabs take positionnal argument which should be a parameter")
    elif len(param) > 1:
        raise RuntimeError("Invalid argument : only one positionnal argument accepted and only when used with a prefab")
    elif len(param) == 0 and isinstance(typeName, type) and issubclass(typeName, BasePrefab):
        raise RuntimeError("Invalid argument : one positionnal argument is required when calling add with prefab type")
    elif len(param) == 1 and isinstance(typeName, type) and issubclass(typeName, BasePrefab) and not isinstance(param[0], BaseParameters):
        raise RuntimeError("Invalid argument : when calling add with prefab type the positionnal argument is expected to be a type derived from stlib.core.BaseParameter")


    if len(param) == 1 and isinstance(typeName, type) and issubclass(typeName, BasePrefab):
        param[0].name = checkName(self, param[0].name)
        if(len(kwargs)):
            param[0].kwargs = kwargs.copy()

        newEntity = self.addChild(typeName(param[0]))
        newEntity.init()
        return newEntity

    ## If we ever get here, it means we are not adding a prefab by giving its type name and its parameter set

    # Check if a name is provided, if not, use the one of the class
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
        elif isinstance(typeName, type) and issubclass(typeName, Sofa.Core.ObjectDeclaration):
            params["name"] = typeName.__name__
        else:
            raise RuntimeError("Invalid argument ", typeName)

    params["name"] = checkName(self, params["name"])

    # Dispatch the creation to either addObject or addChild
    if isinstance(typeName, type) and issubclass(typeName, Sofa.Core.Node):
        pref = self.addChild(typeName(params["name"]))
    elif isinstance(typeName, Sofa.Core.Node):
        pref = self.addChild(typeName)
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
