from stlib.core.entity import Entity
from stlib.core.parameters import EntityParameters
import dataclasses

class Deformable(Entity):
    @dataclasses.dataclass
    class Parameters(EntityParameters):     
        addConstitutiveLaw : Callable = lambda x: x

        mass : Optional[float] = None    

        def to_dict(self):
            return dataclasses.asdict(self)

    def __init__(self, **kwargs):
        Entity.__init__(self, **kwargs)    

