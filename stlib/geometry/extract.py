from stlib.geometry import GeometryParameters, InternalDataProvider, Geometry
from stlib.core.baseParameters import dataclasses
from splib.topology.dynamic import addDynamicTopology
from splib.topology.loader import loadMesh
from splib.core.enum_types import ElementType

from Sofa.Core import Node


class ExtractInternalDataProvider(InternalDataProvider):
    destElementType : ElementType
    fromElemenType : ElementType
    fromNodeName : str

    def __init__(self, destElementType : ElementType, fromElementType : ElementType, fromNodeName : str):
        self.destElementType = destElementType,
        self.fromElementType = fromElementType,
        self.fromNodeName = fromNodeName

    def __post_init__(self):
        if(not (self.fromElementType == ElementType.TETRAHEDRONS and self.destElementType == ElementType.TRIANGLES)
           and not (self.fromElementType == ElementType.HEXAHEDRONS and self.destElementType == ElementType.QUADS) ):
            raise ValueError("Only configuration possible are 'Tetrahedrons to Triangles' and 'Hexahedrons to Quads'")

        InternalDataProvider.__init__(self)


    def generateAttribute(self, parent : Geometry):                
        tmn = parent.addChild("TopologicalMappingNode")

        #TODO: Specify somewhere in the doc that this should only be used for mapped topologies that extract parent topology surface
        
        fromLink = parent.parents[0].parents[0].getChild(self.fromNodeName).container.linkpath
        addDynamicTopology(tmn, type=self.destElementType)
        if self.fromElementType == ElementType.TETRAHEDRONS:
            tmn.addObject("Tetra2TriangleTopologicalMapping", input=fromLink, output=tmn.container.linkpath)
        elif self.fromElementType == ElementType.HEXAHEDRONS:
            tmn.addObject("Hexa2QuadTopologicalMapping", input=fromLink, output=tmn.container.linkpath)

        self.position = tmn.container.position.linkpath
        self.edges = tmn.container.edges.linkpath
        self.triangles = tmn.container.triangles.linkpath
        self.quads = tmn.container.quads.linkpath
        self.hexahedra = tmn.container.hexahedra.linkpath
        self.tetrahedra = tmn.container.tetras.linkpath



class ExtractParameters(GeometryParameters):
    def __init__(self, 
                 sourceParameters : GeometryParameters, 
                 destinationType : ElementType,  
                 dynamicTopology = False, ):
        GeometryParameters.__init__(self,
                                    data = ExtractInternalDataProvider(destElementType = sourceParameters, 
                                                                       fromElementType = destinationType,
                                                                       fromNodeName = destinationType.name), 
                                    dynamicTopology = dynamicTopology, 
                                    elementType = destinationType)
        
