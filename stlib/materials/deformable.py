from stlib.materials import MaterialParameters
from splib.core.enum_types import ConstitutiveLaw
from stlib.core.baseParameters import Callable, Optional
from splib.mechanics.linear_elasticity import *
from splib.mechanics.hyperelasticity import *
from splib.mechanics.mass import addMass


class DeformableMaterialParameters(MaterialParameters):

    constitutiveLawType : ConstitutiveLaw = ConstitutiveLaw.ELASTIC
    elementType : ElementType = ElementType.TETRAHEDRA
    parameters : list[float] = [1000, 0.45]  # young modulus, poisson ratio

    def __addDeformableMaterial(node):

        massKwargs = {}
        if node.parameters.elementType != ElementType.EDGES: #If we use the MeshMatrixMass, then the mass will need us to specify the mstate to use
            massKwargs["geometryState"] = "@States"

        addMass(node, node.parameters.elementType, massDensity=node.parameters.massDensity, lumping=node.parameters.massLumping, topology="@../Geometry/container", mass=massKwargs)
        # TODO : change this with inheritance
        if(node.parameters.constitutiveLawType == ConstitutiveLaw.HYPERELASTIC):
            addHyperelasticity(node, node.parameters.elementType, node.parameters.parameters, topology="@../Geometry/container")
        else:
            addLinearElasticity(node, node.parameters.elementType, node.parameters.parameters[0], node.parameters.parameters[1], topology="@../Geometry/container")

    addMaterial : Optional[Callable] = __addDeformableMaterial

