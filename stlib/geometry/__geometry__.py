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
        
        self.parameters = parameters 

        # Generate attribute (positions, edges, triangles, quads, tetrahedra, hexahedra) from the internal data provider
        if parameters.data is not None :
            parameters.data.generateAttribute(self)
        if parameters.dynamicTopology :
            if parameters.elementType is not None :
                addDynamicTopology(self, container = dataclasses.asdict(parameters.data))
            else:
                raise ValueError
        else:
            addStaticTopology(self, 
                              container = 
                              {
                                "position": parameters.data.position,
                                "edges": parameters.data.edges,
                                "triangles": parameters.data.triangles,
                                "quads": parameters.data.quads,
                                "tetrahedra": parameters.data.tetrahedra,
                                "hexahedra": parameters.data.hexahedra
                              })
