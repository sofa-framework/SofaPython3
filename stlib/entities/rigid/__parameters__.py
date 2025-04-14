from stlib.core.baseParameters import BaseParameters, Callable, Optional, dataclasses

@dataclasses.dataclass
class DRigidParameters(BaseParameters):     
    addConstitutiveLaw : Callable = lambda x: x

    mass : Optional[float] = None    

    def toDict(self):
        return dataclasses.asdict(self)
