from stlib.entities import Entity, EntityParameters
from stlib.modifiers import SingleEntityModifier
from stlib.modifiers.fixing import FixingModifierParameters
from stlib.settings.simulation import addSimulationSettings


def createScene(rootnode):

    simulation = addSimulationSettings(rootnode)

    cube1 = simulation.add(Entity, parameters=EntityParameters(name = "FixedCube"))
    cube2 = simulation.add(Entity, parameters=EntityParameters(name = "FallingCube"))
    
    simulation.create(SingleEntityModifier, parameters=FixingModifierParameters()).apply(entity=cube1)