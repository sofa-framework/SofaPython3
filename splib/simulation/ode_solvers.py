from splib.core.node_wrapper import ReusableMethod

@ReusableMethod
def addImplicitODE(node,static=False,**kwargs):
    if( not(static) ):
        node.addObject("EulerImplicitSolver",name="ODESolver",**kwargs)
    else:
        node.addObject("StaticSolver",name="ODESolver",**kwargs)

@ReusableMethod
def addExplicitODE(node,**kwargs):
    node.addObject("EulerExplicitSolver",name="ODESolver",**kwargs)


