import Sofa

## Pyggy patch ##############################################################
class Parameters(object):
    name : str = ""
    def __init__(self, **kwargs):
        print(f"parameters {kwargs}")
        for k,v in kwargs.items():
            setattr(self, k, v)

    def toDict(self):
        return {"name":self.name}

class ModifierParameters(Parameters):
    def __init__(self, **kwargs):
        Parameters.__init__(self, **kwargs)

    def toDict(self):
        return Parameters.toDict(self) 

def getpythonCallingPoint(depth=0):
    import inspect
    d = inspect.stack()[depth]
    return (d.filename, d.positions.lineno)

class Modifier(Sofa.Core.Controller):
    def __init__(self, parameters : ModifierParameters = ModifierParameters()):
        Sofa.Core.Controller.__init__(self, **parameters.toDict())         

    def getModifiedNode(self):
        return self.getOwner().parents[0]
    
Sofa.Core.Modifier = Modifier

def addModifier(self, type, parameters:Parameters, **kwargs) -> Modifier:
    print("Add modifier ")
    if "Modifiers" not in self.children:
        self.addChild("Modifiers")
    o = self.Modifiers.addObject(type(parameters=parameters, **kwargs))
    o.apply()
    return o
Sofa.Core.Node.addModifier = addModifier 

def add(self, type, parameters:Parameters) -> Sofa.Core.Base:
    print("Add object")
    if issubclass(type, Sofa.Core.Node):
        return self.addChild(type(parameters)) 
Sofa.Core.Node.add = add    

#############################################################################
class PrefabParameters(Parameters):
    def __init__(self, **kwargs):
        Parameters.__init__(self, **kwargs)

    def toDict(self):
        return Parameters.toDict(self) 

class Prefab(Sofa.Core.Node):
    def __init__(self, parameters):
        Sofa.Core.Node.__init__(self, name=parameters.name)

class MyEntityParameters(PrefabParameters):
    def __init__(self, **kwargs):
        PrefabParameters.__init__(self, **kwargs)

    def toDict(self):
        return PrefabParameters.toDict(self) 

class MyEntity(Prefab):
    def __init__(self, parameters):
        Prefab.__init__(self, PrefabParameters(name=parameters.name))

        self.addChild("Material")
        self.addChild("Geometry")
        self.addChild("Collision")
        self.addChild("Visual")
        self.addChild("Modifiers")

        self.Geometry.addObject("MeshOBJLoader", filename="mesh/cube.obj", name="loader")

        self.Material.addObject("MechanicalObject", name="state", template="Vec3", position=self.Geometry.loader.position.linkpath)
        self.Material.addObject("UniformMass", name="mass", totalMass=1.0)

        self.Visual.addObject("OglModel", src=self.Geometry.loader.linkpath)


class FixedPointBoundaryConditionParameters(ModifierParameters):
    def __init__(self, **kwargs):
        ModifierParameters.__init__(self, **kwargs)

    def toDict(self):
        return ModifierParameters.toDict(self) 

class FixedPointBoundaryCondition(Modifier):
    def __init__(self, parameters : FixedPointBoundaryConditionParameters = FixedPointBoundaryConditionParameters()):
        Modifier.__init__(self, parameters=parameters)
    
    def apply(self):
        self.getModifiedNode().Material.addObject("UnilateralInteractionConstraint")

class BilateralInteractionModifierParameters(ModifierParameters):
    object1: Sofa.Core.Object = None
    object2: Sofa.Core.Object = None
      
    def __init__(self, **kwargs):
        ModifierParameters.__init__(self, **kwargs)
        
    def toDict(self):
        return ModifierParameters.toDict(self) | {"object1":self.object1,"object2":self.object2}

class BilateralInteractionModifier(Modifier):
    def __init__(self, parameters : BilateralInteractionModifierParameters = BilateralInteractionModifierParameters()):
        Modifier.__init__(self, parameters=parameters)
        self.object1 = parameters.object1
        self.object2 = parameters.object2        

        h = getpythonCallingPoint(3)
        self.setInstanciationSourceFileName(h[0])
        self.setInstanciationSourceFilePos(h[1])

    def apply(self):
        a = self.object1.Material.addObject("UnilateralInteractionConstraint")
        b = self.object2.Material.addObject("UnilateralInteractionConstraint")

        a.setInstanciationSourceFileName(self.getInstanciationSourceFileName())
        a.setInstanciationSourceFilePos(self.getInstanciationSourceFilePos())

        b.setInstanciationSourceFileName(self.getInstanciationSourceFileName())
        b.setInstanciationSourceFilePos(self.getInstanciationSourceFilePos())

class SolversFromSceneModifierParameters(ModifierParameters):
    root : Sofa.Core.Node = None

    def __init__(self, **kwargs):
        ModifierParameters.__init__(self, **kwargs)
        
    def toDict(self):
        return ModifierParameters.toDict(self) | {"root":self.root}

def find_component(cond, node):
    c = [component for component in node.objects if cond(component)]
    for child in node.children:
        c += find_component(cond, child)
    return c

class SolverFromSceneModifier(Modifier):
    def __init__(self, parameters : SolversFromSceneModifierParameters = SolversFromSceneModifierParameters()):
        Modifier.__init__(self, parameters=parameters)
        self.root = parameters.root

        h = getpythonCallingPoint(3)
        self.setInstanciationSourceFileName(h[0])
        self.setInstanciationSourceFilePos(h[1])

    def apply(self):
        # Traverse scene to deduce stuff 
        constraints = find_component(lambda x: x.getClassName() == "Lagrangian", self.root)
        constraints = [constraint.name.value for constraint in constraints]

        print("I'm modifing ", constraints)
        if "UnilateralLagrangianConstraint" in constraints:
            self.root.addObject("GenericConstraintCorrection")

class HeaderToSceneModifierParameters(ModifierParameters):
    root : Sofa.Core.Node = None

    def __init__(self, **kwargs):
        ModifierParameters.__init__(self, **kwargs)
        
    def toDict(self):
        return ModifierParameters.toDict(self) | {"root":self.root}

class HeaderToSceneModifier(Modifier):
    def __init__(self, parameters : SolversFromSceneModifierParameters = SolversFromSceneModifierParameters()):
        Modifier.__init__(self, parameters=parameters)
        self.root = parameters.root

    def apply(self):
        # Traverse scene to deduce stuff 

        def is_mechanical(object):
            return object.getClassName() == "MechanicalObject"

        mechanicals = find_component(is_mechanical, self.root)
        for mechanical in mechanicals:
            mechanical.getOwner().addObject("GenericConstraintCorrection")            

def createScene(root):
    # Add a modifier in Example1/prefab, it modify the node it is applied to 
    # The use case is to enrich an object
    root.addChild("Example1")
    root.Example1.add(MyEntity, parameters=MyEntityParameters(name="prefab"))
    root.Example1.prefab.addModifier(FixedPointBoundaryCondition, parameters=FixedPointBoundaryConditionParameters(name="boundary1", stiffness=30.0))

    # Add a modifier in Example2, it modify a specific pair of node
    # The use case is to explicit an action that involve several possibly disconnected in the graph object
    # Personnally I'm not sure I want to express BoundaryCondition as Modifiers. 
    root.addChild("Example2")
    root.Example2.add(MyEntity, parameters=MyEntityParameters(name="prefab1"))
    root.Example2.add(MyEntity, parameters=MyEntityParameters(name="prefab2"))
    root.Example2.addModifier(BilateralInteractionModifier, parameters=BilateralInteractionModifierParameters(name="constraint", 
                                                                                                              object1=root.Example2.prefab1,
                                                                                                              object2=root.Example2.prefab2))

    # Add a modifier in the root node, this modifier deduce something from the scene and modify anywhere in the scene. 
    root.addModifier(SolverFromSceneModifier, parameters=SolversFromSceneModifierParameters(name="bottom-up-fix", 
                                                                                            root=root))

    # Add a modifier in the root node, this modifier deduce something from the scene and modify top down the scene. 
    root.addModifier(HeaderToSceneModifier, parameters=HeaderToSceneModifierParameters(name="top-down-fix", 
                                                                                          root=root))

