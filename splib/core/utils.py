from typing import List, Callable, Tuple, Dict
from functools import wraps
import Sofa
import Sofa.Core

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

REQUIRES_COLLISIONPIPELINE = "requiresCollisionPipeline"

def setRequiresCollisionPipeline(rootnode):
    if rootnode is not None: 
        if rootnode.findData(REQUIRES_COLLISIONPIPELINE) is None:
            rootnode.addData(name=REQUIRES_COLLISIONPIPELINE, type="bool", default=False, help="Some prefabs in the scene requires a collision pipeline.")
        else:
            rootnode.requiresCollisionPipeline.value = True

REQUIRES_LAGRANGIANCONSTRAINTSOLVER = "requiresLagrangianConstraintSolver"

def setRequiresLagrangianConstraintSolver(rootnode):
    if rootnode is not None: 
        if rootnode.findData(REQUIRES_LAGRANGIANCONSTRAINTSOLVER) is None:
            rootnode.addData(name=REQUIRES_LAGRANGIANCONSTRAINTSOLVER, type="bool", default=False, help="Some prefabs in the scene requires a Lagrangian constraint solver.")
        else:
            rootnode.requiresLagrangianConstraintSolver.value = True




