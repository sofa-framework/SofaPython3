from stlib.core.baseParameters import BaseParameters, Callable, Optional, dataclasses

@dataclasses.dataclass
class RigidParameters(BaseParameters):     

    mass : Optional[float] = None    

    def toDict(self):
        return dataclasses.asdict(self)
