
import dataclasses
from stlib.core.base_parameters import BaseParameters

@dataclasses.dataclass
class DeformableParameters(BaseParameters):     
    addConstitutiveLaw : Callable = lambda x: x

    mass : Optional[float] = None    

    def to_dict(self):
        return dataclasses.asdict(self)
