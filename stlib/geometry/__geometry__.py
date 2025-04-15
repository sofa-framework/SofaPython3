from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters, Optional, dataclasses, Any
from splib.topology.dynamic import addDynamicTopology
from splib.topology.static import addStaticTopology
from splib.core.enum_types import ElementType
from splib.core.utils import DEFAULT_VALUE
from Sofa.Core import Object 


class Geometry(BasePrefab):...

@dataclasses.dataclass
class InternalDataProvider(object):
    positions : Any
    # Topology information
    edges : Any = DEFAULT_VALUE
    triangles : Any  = DEFAULT_VALUE
    quads : Any  = DEFAULT_VALUE
    tetrahedra : Any  = DEFAULT_VALUE
    hexahedra : Any  = DEFAULT_VALUE

    def generateAttribute(self, parent : Geometry):
        pass


@dataclasses.dataclass
class GeometryParameters(BaseParameters):

    # Type of the highest degree element
    elementType : Optional[ElementType]

    dynamicTopology : bool = False
    data : Optional[InternalDataProvider]



class Geometry(BasePrefab):
    container :  Object # This should be more specialized into the right SOFA type
    modifier : Optional[Object]

    params : GeometryParameters

    def __init__(self, params: GeometryParameters):
        BasePrefab.__init__(self, params)
        self.params = params 
        if params.data is not None :
            params.data.generateAttribute(self)
        if(params.dynamicTopology):
            if(params.elementType is not None):
                addDynamicTopology(self, container = {dataclasses.asdict(params.data)})
            else:
                raise ValueError
        else:
            addStaticTopology(self, container = {dataclasses.asdict(params.data)})
