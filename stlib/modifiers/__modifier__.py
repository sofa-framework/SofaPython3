from stlib.core.baseParameters import BaseParameters
from stlib.core.baseEntityModifier import BaseEntityModifier
from stlib.entities import Entity
import dataclasses


@dataclasses.dataclass
class ModifierParameters(BaseParameters): 

    name : str = "Modifier"
    creator : str = None

    def modifier(entity: Entity):
        pass


class SingleEntityModifier(BaseEntityModifier):

    def __init__(self, parameters: ModifierParameters):
        BaseEntityModifier.__init__(self, parameters)

    def _apply(self, entity: Entity):
        entity.add(self)
        self.parameters.modifier(entity)