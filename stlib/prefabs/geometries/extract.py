from stlib.prefabs.geometries import GeometryParameters, InternalDataProvider, Geometry
from stlib.core.baseParameters import dataclasses
from splib.topology.dynamic import addDynamicTopology
from splib.topology.loader import loadMesh
from splib.core.enum_types import ElementType

import Sofa
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
        # fromLink = parent.parents[0].parents[0].getChild(self.SourceName).container.linkpath
        # TODO: the line above cannot work if the nodes and objects are not added to the graph prior the end of __init__() call
        # !!! also, on a fail, nothing is added to the graph, which makes things harder to debug
        # !!! also, does not work because of the function canCreate(), which checks the input (not yet created?)
        # this is all related
        fromLink = "@../../Geometry.container" # TODO: can we do better than this?
        addDynamicTopology(node, elementType=self.sourceType)
        if self.sourceType == ElementType.TETRAHEDRA:
            node.addObject("Tetra2TriangleTopologicalMapping", input=fromLink, output=node.container.linkpath)
        elif self.sourceType == ElementType.HEXAHEDRA:
            node.addObject("Hexa2QuadTopologicalMapping", input=fromLink, output=node.container.linkpath)
        else:
            Sofa.msg_error("[stlib/geometry/exctrat.py]", "Element type: " + str(self.sourceType) + " not supported.")

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
        
