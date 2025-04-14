from stlib.core.parameters import EntityParameters 
import dataclasses
import Sofa

class VisualModel(Sofa.Core.BasePrefab):
    @dataclasses.dataclass
    class Parameters(EntityParameters):
        name : str = "VisualModel"

        filename : str = "mesh/sphere_02.obj"
        
        renderer : dict = dataclasses.field(default_factory=dict)
        mapping : dict = dataclasses.field(default_factory=dict)



    def __init__(self, parent=None, parameters : VisualModelParameters = VisualModelParameters()):
        Sofa.Core.Node.__init__(self, name=parameters.name)

        if parent != None:
            parent.addChild(self)

        self.addObject("MeshOBJLoader", name="loader", filename=parameters.filename)
        self.addRenderer(**to_dict(parameters.renderer) | {"src" : "@loader"} )
        self.addMapping(**to_dict(parameters.mapping) )

    def addRenderer(self, **kwargs):
        self.addObject("OglModel", name="renderer", **kwargs)

    def addMapping(self, **kwargs):
        self.addObject("RigidMapping", name="mapping", **kwargs)
        