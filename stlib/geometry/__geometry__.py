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
    position : Any = None
    # Topology information
    edges      : Any = DEFAULT_VALUE
    triangles  : Any = DEFAULT_VALUE
    quads      : Any = DEFAULT_VALUE
    tetrahedra : Any = DEFAULT_VALUE
    hexahedra  : Any = DEFAULT_VALUE

    def generateAttribute(self, parent : Geometry):
        pass


@dataclasses.dataclass
class GeometryParameters(BaseParameters):
    name : str = "Geometry"

    # Type of the highest degree element
    elementType : Optional[ElementType] = None
    data : Optional[InternalDataProvider] = None

    dynamicTopology : bool = False


class Geometry(BasePrefab):
    # container : Object # This should be more specialized into the right SOFA type
    # modifier : Optional[Object]

    parameters : GeometryParameters

    def __init__(self, parameters: GeometryParameters):
        BasePrefab.__init__(self, parameters)
        

    
    def init(self):

        # Generate attribute (positions, edges, triangles, quads, tetrahedra, hexahedra) from the internal data provider
        if self.parameters.data is not None :
            self.parameters.data.generateAttribute(self)
        if self.parameters.dynamicTopology :
            if self.parameters.elementType is not None :
                addDynamicTopology(self, container = dataclasses.asdict(self.parameters.data))
            else:
                raise ValueError
        else:
            addStaticTopology(self, 
                              container = 
                              {
                                "position": self.parameters.data.position,
                                "edges": self.parameters.data.edges,
                                "triangles": self.parameters.data.triangles,
                                "quads": self.parameters.data.quads,
                                "tetrahedra": self.parameters.data.tetrahedra,
                                "hexahedra": self.parameters.data.hexahedra
                              })