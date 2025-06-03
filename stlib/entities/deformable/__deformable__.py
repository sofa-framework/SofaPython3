from stlib.entities import Entity, EntityParameters
from stlib.prefabs.material import Material, MaterialParameters
from stlib.prefabs.visual import Visual
from splib.core.enum_types import ConstitutiveLaw, ElementType
from splib.mechanics.linear_elasticity import *
from splib.mechanics.hyperelasticity import *
from splib.mechanics.mass import addMass


class DeformableBehaviorParameters(MaterialParameters):

    constitutiveLawType : ConstitutiveLaw 
    elementType : ElementType
    parameters : list[float]

    def addMaterial(self, node):

        addMass(node, node.stateType, massDensity=node.massDensity, lumping=node.massLumping)

        # TODO : change this with inheritance
        if(self.constitutiveLawType == ConstitutiveLaw.HYPERELASTIC):
            addHyperelasticity(node,self.elementType, self.parameters)
        else:
            addLinearElasticity(node,self.elementType, self.parameters[0], self.parameters[1])


# class Deformable(Entity):

#     params : DeformableParameters

#     @staticmethod
#     def getParameters(**kwargs) -> DeformableParameters:
#         return DeformableParameters(**kwargs)
        

#     def __init__(self, params : DeformableParameters, **kwargs):
#         Entity.__init__(self, **kwargs)  
        
#         self.__addConstitutiveLaw__()
#         self.addCollision(params.collision)


#     #@customizable
#     # Need generic way of defining paramaters (linear/hyper...)
#     def __addConstitutiveLaw__(self):
#         self.params.addConstitutiveLaw()

    
#     #@customizable
#     def __addVisual__(self):
#         #Extract surface and add identity mapping
#         self.add(Visual, self.params.visual)





def createScene(root) :

    from stlib.geometry.file import FileParameters    
    # from stlib.geometry.extract import ExtractParameters    

    liverParameters = EntityParameters()
    liverParameters.behavior = DeformableBehaviorParameters()
    liverParameters.behavior.constitutiveLawType = ConstitutiveLaw.ELASTIC
    liverParameters.behavior.parameters = [1000, 0.45]
    liverParameters.geometry = FileParameters("liver.vtk")
    # liverParameters.visual = ExtractParameters()
    myDeformableObject = root.add(Entity, liverParameters)