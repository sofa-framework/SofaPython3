from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters, Optional, dataclasses
from splib.topology.loader import loadMesh
from splib.topology.dynamic import addDynamicTopology
from splib.core.enum_types import ElementType
from Sofa.Core import Object 



@dataclasses.dataclass
class GeometryParameter(BaseParameters):
    isDynamic : bool = False
    elementType : Optional[ElementType]
    positions : Optional[list]
    edges : Optional[list]
    triangles : Optional[list]
    tetrahedra : Optional[list]
    hexahedra : Optional[list]
    quads : Optional[list]


class Geometry(BasePrefab):
    container :  Object # This should be more specialized into the right SOFA type
    modifier : Optional[Object]

    def __init__(self, params: GeometryParameter):
        BasePrefab.__init__(self, params)
        if(param.isDynamic):
            if(param.elementType is not None):
                addDynamicTopology(self, param.elementType, **kargs)
            else:
                raise ValueError


@dataclasses.dataclass
class FileGeometryParameters(GeometryParameter):
    filename : str


class FileGeometry(Geometry):
    loader : Object

    def __init__(self,param : FileGeometryParameters):
        loadMesh(self, param.filename, **param.kwargs)
        param.positions = "@loader.positions"
        param.edges = "@loader.edges"
        param.positions = "@loader.positions"
        param.positions = "@loader.positions"
        Geometry.__init__(self,param)

        pass



def createScene(root):
    from stlib.entities.deformable import Deformable 

    fileParameters =  FileGeometryParameters(filename = "MyFile.obj",
                                             isDynamic = True,
                                             elementType=ElementType.TETRA,
                                             name="geometry")



    cubeGeometry = FileGeometry(FileGeometryParameters)

    entityParam = Deformable.getParameters( geometry = cubeGeometry, name="MyCube")


    myCube = root.addChild("MyCube")
    cubeGeomtry = myCube.addChild("geometry")

    cubeGeomtry.addObject("MeshObjLoader", name="loader", filenmae="MyFile.obj")
    cubeGeomtry.addObject("TetrahedronSetTopologyContainer", name="container", positions="@loader.positions")
    cubeGeomtry.addObject("TetrahedronSetTopologyModifier", name="modifier")
    
