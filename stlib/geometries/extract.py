from stlib.geometries import GeometryParameters, InternalDataProvider, Geometry
from stlib.core.baseParameters import dataclasses
from splib.topology.dynamic import addDynamicTopology
from splib.topology.loader import loadMesh
from splib.core.enum_types import ElementType

import Sofa
from Sofa.Core import Node
from functools import partial


class ExtractInternalDataProvider(InternalDataProvider):

    def __init__(self):
        super().__init__()

    def generateAttribute(self, parent : Geometry):
        self.position = parent.parents[0].parents[0].getChild("Geometry").container.position.linkpath


def extractGeometry(sourceType : ElementType, parent : Geometry):
    #TODO: Specify somewhere in the doc that this should only be used for mapped topologies that extract parent topology surface
    # fromLink = parent.parents[0].parents[0].getChild(self.SourceName).container.linkpath
    # TODO: the line above cannot work if the nodes and objects are not added to the graph prior the end of __init__() call
    # !!! also, on a fail, nothing is added to the graph, which makes things harder to debug
    # !!! also, does not work because of the function canCreate(), which checks the input (not yet created?)
    # this is all related
    fromLink = parent.parents[0].parents[0].getChild("Geometry").container.linkpath # TODO: can we do better than this?
    if sourceType == ElementType.TETRAHEDRA:
        parent.addObject("Tetra2TriangleTopologicalMapping", input=fromLink, output=parent.container.linkpath)
    elif sourceType == ElementType.HEXAHEDRA:
        parent.addObject("Hexa2QuadTopologicalMapping", input=fromLink, output=parent.container.linkpath)
    else:
        Sofa.msg_error("[stlib/geometry/exctrat.py]", "Element type: " + str(sourceType) + " not supported.")




class ExtractParameters(GeometryParameters):
    def __init__(self,
                 sourceParameters : GeometryParameters,
                 destinationType : ElementType):
        GeometryParameters.__init__(self,
                                    data = ExtractInternalDataProvider(),
                                    dynamicTopology = True,
                                    elementType = destinationType)

        self.postInit = partial(extractGeometry, sourceParameters.elementType)

        if(not (sourceParameters.elementType == ElementType.TETRAHEDRA and destinationType == ElementType.TRIANGLES)
            and not (sourceParameters.elementType == ElementType.HEXAHEDRA and destinationType == ElementType.QUADS) ):
            raise ValueError("Only configuration possible are 'Tetrahedra to Triangles' and 'Hexahedra to Quads'")

