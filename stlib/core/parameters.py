import dataclasses
from splib.core.utils import DEFAULT_VALUE

#defaultValueType

@dataclasses.dataclass
class EntityParameters(object): 
    name : str = "Prefab"
    kwargs : dict = dataclasses.field(default_factory=dict)

    def __getattr__(self, name: str) :
        if name == "__getstate__":
            getattr(PrefabParameters, "__getstate__")
        if name == "__setstate__":
            getattr(PrefabParameters, "__setstate__")

        try: 
            a =  self.__getattribute__(name)
        except Exception as e: 
            return DEFAULT_VALUE
        return a

    def to_dict(self):
        return dataclasses.asdict(self)
