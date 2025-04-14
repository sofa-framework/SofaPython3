from splib.core.node_wrapper import ReusableMethod
from splib.core.utils import DEFAULT_VALUE

@ReusableMethod
def addStaticTopology(node,source=DEFAULT_VALUE,**kwargs):
    node.addObject("MeshTopology", name="container", src=source, **kwargs)

