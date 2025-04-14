from splib.core.node_wrapper import ReusableMethod
from splib.core.utils import *

@ReusableMethod
def addLinearSolver(node,iterative=False,iterations=DEFAULT_VALUE,tolerance=DEFAULT_VALUE,threshold=DEFAULT_VALUE,template=DEFAULT_VALUE,constantSparsity=False,parallelInverseProduct=DEFAULT_VALUE,**kwargs):
    containerParams = getParameterSet("LinearSolver",kwargs)
    if iterative:
        if not isDefault(iterations):
            containerParams["iterations"] = iterations
        if not isDefault(tolerance):
            containerParams["tolerance"] = tolerance
        if not isDefault(threshold):
            containerParams["threshold"] = threshold
    else:
        if isDefault(template) and not("template" in containerParams):
            containerParams["template"] = 'CompressedRowSparseMatrix'
        elif not isDefault(template) :
            containerParams["template"] = template
    kwargs["LinearSolver"] = containerParams

    if(constantSparsity):
        node.addObject("ConstantSparsityPatternSystem",name='LinearSystem',**kwargs)
        kwargs["LinearSolver"]["template"] = 'CompressedRowSparseMatrix'
        kwargs["LinearSolver"]["linearSystem"]="@LinearSystem"



    if iterative:
        node.addObject('CGLinearSolver', name='LinearSolver', parallelInverseProduct=parallelInverseProduct, **kwargs)
    else:
        node.addObject('SparseLDLSolver', name='LinearSolver', parallelInverseProduct=parallelInverseProduct, **kwargs)