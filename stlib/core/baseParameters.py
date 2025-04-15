import dataclasses
from splib.core.utils import DEFAULT_VALUE

import dataclasses
from typing import Callable, Optional, Any

@dataclasses.dataclass
class BaseParameters(object): 
    name : str = "object"
    kwargs : dict = dataclasses.field(default_factory=dict)

    def toDict(self):
        return dataclasses.asdict(self)
