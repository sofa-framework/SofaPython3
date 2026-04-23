from splib.core.utils import DEFAULT_VALUE

from pydantic import BaseModel, ValidationError
from dynapydantic import SubclassTrackingModel
from typing import Callable, Optional, Any
import Sofa

class BaseParameters(SubclassTrackingModel,
                     discriminator_field="type",
                     discriminator_value_generator=lambda t: t.__name__,): 

    name : str = "Object"
    kwargs : dict = {}

    @classmethod
    def fromYaml(self, data: str):
        import yaml
        dataDict = yaml.safe_load(data)
        return self.fromDict(dataDict)

    @classmethod
    def fromDict(self, data: dict):
        try:
            return self.model_validate(data, strict=True)
        except ValidationError as exc:
            for error in exc.errors():
                loc = error.get("loc")
                message = ""
                for locPart in loc:
                    message += locPart.__str__() + ": "
                message += error.get("msg") 
                Sofa.msg_error(self.__name__, message)


