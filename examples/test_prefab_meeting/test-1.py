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

    params = entity.Deformable.Parameters()
    params.name = "Deformable2"
    root.add(entity.Deformable, params=auto_load)     

    #def addCustomVisual(self, **kwargs):
    #    Rigid.addVisualModel( mapping={"toto":"in"} )

    #params = Entity.Parameters()
    #params.addVisualModel = addCustomVisual    
    #root.add(Entity, params)     

    # 
    #params = Rigid.new_parameters()
    #params.mass = 4.5
    #root.add(Entity, params) 
    #root.add(Entity)     

    #params.addVisualModelOverride = addCustomVisual
    
    ### 
    #Entity._addVisualModel = addCustomVisual 
    #root.add(Entity, params)     
    
    #root.add(Entity.Rigid)     
    #root.add(Entity.Deformable)     

    #root.add(Entity)
    #root.add(VisualModel, params)

    #root.add(VisualModel)

    #params = Entity.Deformable.Parameters()
    #params.visual = None 
    #a = root.add(Entity.Deformable, params)
    
    return root