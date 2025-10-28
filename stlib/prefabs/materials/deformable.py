from stlib.prefabs.materials import MaterialParameters
from splib.core.enum_types import ConstitutiveLaw
from stlib.core.baseParameters import Callable, Optional, dataclasses
from splib.mechanics.linear_elasticity import *
from splib.mechanics.hyperelasticity import *
from splib.mechanics.mass import addMass


@dataclasses.dataclass
class DeformableBehaviorParameters(MaterialParameters):

    constitutiveLawType : ConstitutiveLaw = ConstitutiveLaw.ELASTIC
    elementType : ElementType = ElementType.TETRAHEDRA
    parameters : list[float] = dataclasses.field(default_factory=lambda: [1000, 0.45])  # young modulus, poisson ratio

    def __addDeformableMaterial(node):
        addMass(node, node.parameters.elementType, massDensity=node.parameters.massDensity, lumping=node.parameters.massLumping)
        # TODO : change this with inheritance
        if(node.parameters.constitutiveLawType == ConstitutiveLaw.HYPERELASTIC):
            addHyperelasticity(node, node.parameters.elementType, node.parameters.parameters, topology="@../Geometry/container")
        else:
            addLinearElasticity(node, node.parameters.elementType, node.parameters.parameters[0], node.parameters.parameters[1], topology="@../Geometry/container")

    addMaterial : Optional[Callable] = __addDeformableMaterial


def createScene(root) :
    from stlib.entities import Entity, EntityParameters
    from stlib.prefabs.visual import VisualParameters
    from stlib.geometry import FileParameters

    root.addObject("VisualStyle", displayFlags=["showBehavior"])

    bunnyParameters = EntityParameters()
    bunnyParameters.geometry = FileParameters(filename="mesh/Bunny.vtk")
    bunnyParameters.geometry.elementType = ElementType.TETRAHEDRA # TODO: this is required by extract.py. Should it be done automatically in geometry.py ?
    bunnyParameters.material = DeformableBehaviorParameters()
    bunnyParameters.material.constitutiveLawType = ConstitutiveLaw.ELASTIC
    bunnyParameters.material.parameters = [1000, 0.45]
    bunnyParameters.visual = VisualParameters()
    # bunnyParameters.visual.geometry = ExtractParameters(sourceParameters=bunnyParameters.geometry,
    #                                                     destinationType=ElementType.TRIANGLES)
    bunnyParameters.visual.geometry = FileParameters(filename="mesh/Bunny.stl")
    bunnyParameters.visual.color = [1, 1, 1, 0.5]
    bunny = root.add(Entity, bunnyParameters)