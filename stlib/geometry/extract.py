from stlib.geometry import GeometryParameters, InternalDataProvider, Geometry
from stlib.core.baseParameters import dataclasses
from splib.topology.dynamic import addDynamicTopology
from splib.topology.loader import loadMesh
from splib.core.enum_types import ElementType

from Sofa.Core import Node

#TODO enable passing kwargs to this to use addObject instead and again get the benefit from splib
class ExtractInternalDataProvider(InternalDataProvider):
    destElemType : ElementType
    fromElemType : ElementType
    fromNodeName : str

    def __post_init__(self):
        if(not (self.fromElemType == ElementType.TETRA and self.destElemType == ElementType.TRIANGLES)
           and not (self.fromElemType == ElementType.HEXA and self.destElemType == ElementType.QUAD) ):
            raise ValueError("Only configuration possible are 'Tetra to Triangle' and 'Hexa to quad'")

        InternalDataProvider.__init__(self)


    def generateAttribute(self, parent : Geometry):                
        tmn = parent.addChild("topologicalMappingNode")

        #TODO: Specify somewhere in the doc that this should only be used for mapped topologies that extract parent topology surface
        fromLink = parent.parents[0].parents[0].getChild(self.fromNodeName).container.linkpath
        addDynamicTopology(tmn, type=self.destElemType)
        if self.fromElemType == ElementType.TETRA:
            tmn.addObject("Tetra2TriangleTopologicalMapping", input=fromLink, output=tmn.container.linkpath)
        elif self.fromElemType == ElementType.HEXA:
            tmn.addObject("Hexa2QuadTopologicalMapping", input=fromLink, output=tmn.container.linkpath)

        self.position = tmn.container.position.linkpath
        self.edges = tmn.container.edges.linkpath
        self.triangles = tmn.container.triangles.linkpath
        self.quads = tmn.container.quads.linkpath
        self.hexahedra = tmn.container.hexahedra.linkpath
        self.tetrahedra = tmn.container.tetras.linkpath



class ExtractParameters(GeometryParameters):
    def __init__(self, fromGeometry : GeometryParameters, destElementType : ElementType,  dynamicTopology = False, ):
        GeometryParameters.__init__(data = ExtractInternalDataProvider(destElemType = destElementType, fromElemType = fromGeometry.elementType,fromNodeName = fromGeometry.name), dynamicTopology = dynamicTopology, elementType = destElementType)
        
