from typing import Callable, Optional, overload
import Sofa.Core


import entity 
import entity2 

# Monkey patch for demonstration purpose
def newAdd(self, creator, **kwargs):
    if callable(creator):
        creator(parent=self, **kwargs)
Sofa.Core.Node.add = newAdd

def createScene(root):
    #root.add(entity.Deformable)     
    #root.addChild(entity2.Deformable(root))     

    parameters = entity.Deformable.Parameters()
    parameters.name = "Deformable2"
    root.add(entity.Deformable, parameters=auto_load)     

    #def addCustomVisual(self, **kwargs):
    #    Rigid.addVisualModel( mapping={"toto":"in"} )

    #parameters = Entity.Parameters()
    #parameters.addVisualModel = addCustomVisual    
    #root.add(Entity, parameters)     

    # 
    #parameters = Rigid.new_parameters()
    #parameters.mass = 4.5
    #root.add(Entity, parameters) 
    #root.add(Entity)     

    #parameters.addVisualModelOverride = addCustomVisual
    
    ### 
    #Entity._addVisualModel = addCustomVisual 
    #root.add(Entity, parameters)     
    
    #root.add(Entity.Rigid)     
    #root.add(Entity.Deformable)     

    #root.add(Entity)
    #root.add(VisualModel, parameters)

    #root.add(VisualModel)

    #parameters = Entity.Deformable.Parameters()
    #parameters.visual = None 
    #a = root.add(Entity.Deformable, parameters)
    
    return root