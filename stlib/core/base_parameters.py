import dataclasses
from splib.core.utils import DEFAULT_VALUE

from stlib.shapes import Shape


@dataclasses.dataclass
class BaseParameters(object): 
    name : str = "Entity"
    shape : Shape = Shape()
    kwargs : dict = dataclasses.field(default_factory=dict)

    def __getattr__(self, name: str) :
        if name == "__getstate__":
            getattr(BaseParameters, "__getstate__")
        if name == "__setstate__":
            getattr(BaseParameters, "__setstate__")

        try: 
            a =  self.__getattribute__(name)
        except Exception as e: 
            return DEFAULT_VALUE
        return a

    def to_dict(self):
        return dataclasses.asdict(self)
