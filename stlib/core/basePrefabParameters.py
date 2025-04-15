import dataclasses

@dataclasses.dataclass
class BasePrefabParameters(object): 
    name : str = "object"
    kwargs : dict = dataclasses.field(default_factory=dict)

    # Transformation information
    # TODO: these data are going to be added in Node in SOFA (C++ implementation)
    translation : list[float] = [0., 0., 0.]
    rotation : list[float] = [0., 0., 0.]
    scale : list[float] = [1., 1., 1.]

    def toDict(self):
        return dataclasses.asdict(self)
