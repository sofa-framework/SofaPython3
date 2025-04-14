from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters, Optional
from splib.topology.loader import loadMesh
from splib.core.enum_types import ElementType
from Sofa.Core import Object 

class Geometry(BasePrefab):
    topology :  Object # This should be more specialized into the right SOFA type
    topologyModifier : Optional[Object]

    def __init__(self, *args, **kwargs):
        BasePrefab.__init__(self, *args, **kwargs)


class GeometryParameter(BaseParameters):
    isDynamic : bool
    elementType : Optional[ElementType]


class FileGeometryParamtetrs(GeometryParameter):
    filename : str


class FileGeometry(Geometry):
    fileLoader : Object

    def __init__(self):
        pass
    