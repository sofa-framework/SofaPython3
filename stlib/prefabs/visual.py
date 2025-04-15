from stlib.core.basePrefab import BasePrefab
from stlib.core.baseParameters import BaseParameters, Callable, Optional, dataclasses, Any
from stlib.geometry import Geometry, GeometryParameters
from Sofa.Core import Object 

@dataclasses.dataclass
class VisualParameters(BaseParameters):
    color : Optional[list[float]]
    texture :  Optional[str]

    geometry : GeometryParameters
    addMapping : Optional[Callable]


class Visual(BasePrefab):
    container :  Object # This should be more specialized into the right SOFA type
    modifier : Optional[Object]

    def __init__(self, params: VisualParameters):
        BasePrefab.__init__(self, params)

        geom = self.add(Geometry, params.geometry)
        # TODO : handle optional color, texture using DEFAULT_VALUE mechanism (as implemented by Paul)
        self.addObject("OglModel", color=params.color, src=geom.container.linkpath)

        if params.addMapping is not None:
            params.addMapping(self)

