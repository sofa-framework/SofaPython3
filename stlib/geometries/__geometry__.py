from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters, Optional, Any
from splib.topology.dynamic import addDynamicTopology
from splib.topology.static import addStaticTopology
from splib.core.enum_types import ElementType
from splib.core.utils import DEFAULT_VALUE
from Sofa.Core import Object

import numpy as np


class Geometry(BasePrefab):...

class InternalDataProvider(BaseParameters):
    position : Any = None
    # Topology information
    edges      : Any = DEFAULT_VALUE
    triangles  : Any = DEFAULT_VALUE
    quads      : Any = DEFAULT_VALUE
    tetrahedra : Any = DEFAULT_VALUE
    hexahedra  : Any = DEFAULT_VALUE

    @classmethod
    def generateAttribute(self, parent : Geometry):
        pass


class GeometryParameters(BaseParameters):
    name : str = "Geometry"

    elementType : Optional[ElementType] = None # Type of the highest degree element
    data : Optional[InternalDataProvider] = None

    dynamicTopology : bool = False


class Geometry(BasePrefab):

    parameters : GeometryParameters

    def __init__(self, parameters: GeometryParameters):
        BasePrefab.__init__(self, parameters)
    
    def init(self):

        # Generate attribute (positions, edges, triangles, quads, tetrahedra, hexahedra) from the internal data provider
        if isinstance(self.parameters.data, InternalDataProvider) :
            self.parameters.data.generateAttribute(self)

        if self.parameters.dynamicTopology :
            if self.parameters.elementType is not None :
                addDynamicTopology(self, 
                                   elementType=self.parameters.elementType, 
                                   container = {
                                                "position": self.parameters.data.position,
                                                "edges": self.parameters.data.edges,
                                                "triangles": self.parameters.data.triangles,
                                                "quads": self.parameters.data.quads,
                                                "tetrahedra": self.parameters.data.tetrahedra,
                                                "hexahedra": self.parameters.data.hexahedra
                                                })
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