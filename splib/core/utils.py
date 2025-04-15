from typing import List, Callable, Tuple, Dict
from functools import wraps
from node_wrapper import ReusableMethod


class defaultValueType():
    def __init__(self):
        pass

DEFAULT_VALUE = defaultValueType()

def isDefault(obj):
    return isinstance(obj,defaultValueType)


def getParameterSet(name : str,parameterSet : Dict) -> Dict:
    if name in parameterSet:
        if isinstance(parameterSet[name], dict):
            return parameterSet[name]
    return {}


def MapKeywordArg(objectName,*argumentMaps):
    def MapArg(method):
        @wraps(method)
        def wrapper(*args, **kwargs):
            containerParams = getParameterSet(objectName,kwargs)
            for argMap in argumentMaps:
                if (argMap[0] in kwargs) and not(kwargs[argMap[0]] is None):
                    containerParams[argMap[1]] = kwargs[argMap[0]]
                kwargs[objectName] = containerParams
            return method(*args,**kwargs)
        return wrapper
    return MapArg


@ReusableMethod
def addObject(node,typeName : str, name : str , **kwargs):
    node.addObject(typeName, name = name, **kwargs)



