from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters, Optional, dataclasses, Any
from splib.topology.dynamic import addDynamicTopology
from splib.topology.static import addStaticTopology
from splib.core.enum_types import ElementType
from Sofa.Core import Object 

@dataclasses.dataclass
class GeometryParameters(BaseParameters):
    @dataclasses.dataclass
    class Data(object):
        # Initial positions
        positions : Any

        # Type of the highest degree element
        elementType : Optional[ElementType]

        # Topology information
        edges : Optional[Any]
        triangles : Optional[Any]
        quads : Optional[Any]
        tetrahedra : Optional[Any]
        hexahedra : Optional[Any]

    dynamicTopology : bool = False
    data : Data

class Geometry(BasePrefab):
    container :  Object # This should be more specialized into the right SOFA type
    modifier : Optional[Object]

    def __init__(self, params: GeometryParameters):
        BasePrefab.__init__(self, params)
        if(params.dynamicTopology):
            if(params.elementType is not None):
                addDynamicTopology(self, **dataclasses.asdict(params.data), **params.kargs)
            else:
                raise ValueError
        else:
            addStaticTopology(self, **dataclasses.asdict(params.data), **params.kargs)
