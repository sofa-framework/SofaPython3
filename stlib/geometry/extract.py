from stlib.geometry import GeometryParameters, InternalDataProvider, Geometry
from stlib.core.baseParameters import dataclasses
from splib.topology.dynamic import addDynamicTopology
from splib.topology.loader import loadMesh
from splib.core.enum_types import ElementType

from Sofa.Core import Node


class ExtractInternalDataProvider(InternalDataProvider):
    destinationType : ElementType
    sourceType : ElementType
    sourceName : str

    def __init__(self, destinationType : ElementType, sourceType : ElementType, sourceName : str):
        self.destinationType = destinationType
        self.sourceType = sourceType
        self.sourceName = sourceName

    def __post_init__(self):
        if(not (self.sourceType == ElementType.TETRAHEDRA and self.destinationType == ElementType.TRIANGLES)
           and not (self.sourceType == ElementType.HEXAHEDRA and self.destinationType == ElementType.QUADS) ):
            raise ValueError("Only configuration possible are 'Tetrahedra to Triangles' and 'Hexahedra to Quads'")

        InternalDataProvider.__init__(self)


    def generateAttribute(self, parent : Geometry):                
        node = parent.addChild("ExtractedGeometry")

        #TODO: Specify somewhere in the doc that this should only be used for mapped topologies that extract parent topology surface
        print(parent)
        # fromLink = parent.parents[0].parents[0].getChild(self.SourceName).container.linkpath
        fromLink = "@../../Geometry.container"
        addDynamicTopology(node, type=self.destinationType)
        if self.sourceType == ElementType.TETRAHEDRA:
            node.addObject("Tetra2TriangleTopologicalMapping", input=fromLink, output=node.container.linkpath)
        elif self.sourceType == ElementType.HEXAHEDRA:
            node.addObject("Hexa2QuadTopologicalMapping", input=fromLink, output=node.container.linkpath)

        self.position = node.container.position.linkpath
        if node.container.findData("edges") is not None:
            self.edges = node.container.edges.linkpath
        if node.container.findData("triangles") is not None:
            self.triangles = node.container.triangles.linkpath
        if node.container.findData("quads") is not None:
            self.quads = node.container.quads.linkpath
        if node.container.findData("hexahedra") is not None:
            self.hexahedra = node.container.hexahedra.linkpath
        if node.container.findData("tetras") is not None:
            self.tetrahedra = node.container.tetras.linkpath



class ExtractParameters(GeometryParameters):
    def __init__(self, 
                 sourceParameters : GeometryParameters, 
                 destinationType : ElementType,  
                 dynamicTopology : bool = False, ):
        GeometryParameters.__init__(self,
                                    data = ExtractInternalDataProvider(destinationType = destinationType, 
                                                                       sourceType = sourceParameters.elementType,
                                                                       sourceName = sourceParameters.name), 
                                    dynamicTopology = dynamicTopology, 
                                    elementType = destinationType)
        
