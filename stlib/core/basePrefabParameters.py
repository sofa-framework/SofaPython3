from stlib.core.baseParameters import BaseParameters

class BasePrefabParameters(BaseParameters): 
    name : str = "object"
    kwargs : dict = {}

    # Transformation information
    # TODO: these data are going to be added in Node in SOFA (C++ implementation)
    translation : list[float] = [0., 0., 0.]
    rotation : list[float] = [0., 0., 0.]
    scale : list[float] = [1., 1., 1.]
        