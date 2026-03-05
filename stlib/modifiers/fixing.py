from stlib.modifiers import ModifierParameters
from stlib.entities import Entity
from stlib.core.baseParameters import Optional, dataclasses
from splib.core.utils import DEFAULT_VALUE

@dataclasses.dataclass
class FixingModifierParameters(ModifierParameters):

    name : str = "FixingModifier"
    indices : Optional[list[int]] = DEFAULT_VALUE

    def modifier(self, entity: Entity):
        entity.material.addObject("FixedProjectiveConstraint", indices=self.indices)