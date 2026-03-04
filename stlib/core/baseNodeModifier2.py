
import splib.mechanics.fix_points
from splib.core.enum_types import ConstraintType
from stlib.core.baseNodeModifierParameters import BaseNodeModifierParameters, SingleNodeModifierParameters, PairNodeModifierParameters
from splib.core.utils import DEFAULT_VALUE
from stlib.entities import Entity
import Sofa
from Sofa.Core import Node

import sys
import dataclasses

class NodeModifier(Sofa.Core.Controller):

    parameters : BaseNodeModifierParameters

    def __init__(self, parameters : BaseNodeModifierParameters):
        Sofa.Core.Controller.__init__(self, **(parameters.toDict()))
        self.parameters = parameters

    @staticmethod
    def findMostcommonPath( *nodes : Node) -> str :
        if len(nodes) == 1 :
            return str(nodes[0].linkpath)

        nodesParentsTree = []
        minSize = sys.maxsize
        for node in nodes:
            print(node.getLinkPath())
            nodesParentsTree.append(str(node.getLinkPath()).split('/'))
            if len(nodesParentsTree[-1]) < minSize:
                minSize = len(nodesParentsTree[-1])

        for i in range(minSize):
            isCommon = True
            for j in range(1, len(nodesParentsTree)) :
                isCommon = isCommon and nodesParentsTree[j][i] == nodesParentsTree[0][i]
            if not isCommon:
                return '/'.join(nodesParentsTree[0][:i])

        return '/'.join(nodesParentsTree[0][:minSize])


    def register( self, holder, *nodes : Node) -> Node :
        #Find holder path as the most common ancestor if None is given
        if(holder is None):
            path = NodeModifier.findMostcommonPath(*nodes)

            childrenToRoot = path.split('/')
            if(childrenToRoot[0] == '@' ):
                childrenToRoot = childrenToRoot[1:]

            holder = nodes[0].getRoot()
            for child in childrenToRoot:
                if(child == ''):
                    break
                holder = holder.getChild(child)

        #Remove last / if exists (this is only the case for root
        holderPath = str(holder.getLinkPath())
        if holderPath[-1] == "/":
            holderPath = holderPath[:-1]

        #Find Relative paths to nodes on which it is applied + add myself to data
        for node in nodes:
            targetPath = str(node.getLinkPath())
            relPath = targetPath.removeprefix(holderPath)
            relPath =  "@" + relPath.count('/')*"../" + "Modifiers/" + self.name.value


            if node.getData("modifiedBy") is None:
                node.addData("modifiedBy", type="vector<string>",value=[], default=[], help="Modifiers that modified this Node", group="STLIB")

            node.modifiedBy = node.modifiedBy.value + [relPath]

        #Add targets to self data
        for node in nodes:
            targetPath = str(node.getLinkPath())
            relPath = targetPath.removeprefix(holderPath)
            relPath = "@.." + relPath

            if self.getData("modified") is None:
                self.addData("modified", type="vector<string>",value=[], default=[], help="Nodes modified by this modifier", group="STLIB")

            self.modified = self.modified.value + [relPath]

        #Now finally register
        if "Modifiers" not in holder.children:
            holder.addChild("Modifiers")
        holder.Modifiers.addObject(self)



class SingleNodeModifier(NodeModifier):

    def __init__(self, parameters : BaseNodeModifierParameters):
        super().__init__(parameters)

    def modify(self, node : Node, holder : Node = None):
        self.parameters.modifier(node)
        self.register(holder, node)



class PairNodeModifier(NodeModifier):

    def __init__(self, parameters : BaseNodeModifierParameters):
        super().__init__(parameters)

    def modify(self, node1 : Node, node2 : Node, holder : Node = None):
        self.parameters.modifier(node1,node2)
        self.register(holder, node1, node2)



class MultiNodeModifier(NodeModifier):

    def __init__(self, parameters : BaseNodeModifierParameters):
        super().__init__(parameters)

    def modify(self, *nodes : Node , holder : Node = None):
        self.parameters.modifier(*nodes)
        self.register(holder, *nodes)




@dataclasses.dataclass
class FixConstraintParameters(SingleNodeModifierParameters):
    constraintType : ConstraintType = ConstraintType.PROJECTIVE
    boxROI : list[ list[ float ] ] = dataclasses.field(default_factory= [[0,0,0], [1,1,1]])

    def modifier(self, node : Node):
        splib.mechanics.fix_points.addFixation(node.Material, type = self.constraintType, boxROIs = self.boxROI)
        pass



@dataclasses.dataclass
class AttachmentConstraintParameters(PairNodeModifierParameters):
    constraintType : ConstraintType = ConstraintType.PROJECTIVE

    def modifier(self, node1 : Node, node2 : Node):
        # splib.mechanics.fix_points.addFixation(node, type = self.constraintType, boxROI = self.boxROI)
        pass



@dataclasses.dataclass
class SimulationSolversParameters(SingleNodeModifierParameters):
    constraintType : ConstraintType = ConstraintType.PROJECTIVE
    boxROI : list[ list[ float ] ] = DEFAULT_VALUE

    def modifier(self, node1 : Node):
        splib.simulation.ode_solvers.addImplicitODE(node1)
        splib.simulation.linear_solvers.addLinearSolver(node1, constantSparsity=False, )
        pass



@dataclasses.dataclass
class SimulationSettingsParameters(SingleNodeModifierParameters):
    useLagrangian : bool = False

    def modifier(self, node1 : Node):
        if(self.useLagrangian):
            splib.simulation.headers.setupLagrangianCollision(root, displayFlags = "showVisualModels",backgroundColor=[0.8, 0.8, 0.8, 1],
                                     parallelComputing = True,alarmDistance=0.3, contactDistance=0.02,
                                     frictionCoef=0.5, tolerance=1.0e-4, maxIterations=20)
        else:
            splib.simulation.headers.setupDefaultHeader(root, displayFlags = "showVisualModels",
                                                    backgroundColor=[0.8, 0.8, 0.8, 1],
                                                    parallelComputing = True)





if __name__ == "__main__":

    from stlib.geometries.plane import PlaneParameters
    from stlib.geometries.file import FileParameters
    from stlib.geometries.extract import ExtractParameters
    from stlib.materials.deformable import DeformableBehaviorParameters
    from stlib.collision import Collision, CollisionParameters
    from stlib.entities import Entity, EntityParameters
    from stlib.visual import Visual, VisualParameters
    from splib.core.enum_types import CollisionPrimitive, ElementType, ConstitutiveLaw
    from splib.simulation.headers import setupLagrangianCollision, setupDefaultHeader
    from splib.simulation.ode_solvers import addImplicitODE
    from splib.simulation.linear_solvers import addLinearSolver
    from stlib.core.basePrefab import BasePrefab, BasePrefabParameters

    #### The root is a vanilla Node
    root = Sofa.Core.Node(name="root")


    root.addObject("RequiredPlugin", pluginName=[ 'Sofa.Component',
                                                  'Sofa.GL.Component.Rendering3D',
                                                  'SofaImGui'
                                                  ])


    LogoParams = EntityParameters(name = "Logo",
                                  geometry = FileParameters(filename="mesh/SofaScene/Logo.vtk"),
                                  material = DeformableBehaviorParameters(),
                                  collision = CollisionParameters(geometry = FileParameters(filename="mesh/SofaScene/LogoColli.sph")),
                                  visual = VisualParameters(geometry = FileParameters(filename="mesh/SofaScene/LogoVisu.obj")))

    LogoParams.geometry.elementType = ElementType.TETRAHEDRA
    LogoParams.material.constitutiveLawType = ConstitutiveLaw.ELASTIC
    LogoParams.material.parameters = [200, 0.4]
    LogoParams.material.massDensity = 0.003261
    LogoParams.collision.primitives = [CollisionPrimitive.SPHERES]
    #TODO make this flawless with spheres. Here collisions elements are not in the topology and a link is to be made between the loader and the collision object
    LogoParams.collision.kwargs = {"SphereCollision" : {"radius" : "@Geometry/loader.listRadius"}}
    LogoParams.visual.color = [0.7, .35, 0, 0.8]

    Logo = root.add(Entity, parameters = LogoParams)

    SomeChildren = root.addChild("SomeChildren")
    LogoParams.name="Logo1"
    Logo1 = SomeChildren.add(Entity, parameters = LogoParams)
    LogoParams.name="Logo2"
    Logo2 = SomeChildren.add(Entity, parameters = LogoParams)

    ### Now code is clearer
    # Newbies don't need to care, if they don't give the holder then it is set by default to most common ancestor
    # But us that are building the prefabs we will use it with caution and give the right holder
    SingleNodeModifier(SimulationSolversParameters(name="solvers")).modify(Logo, holder=root)
    SingleNodeModifier(FixConstraintParameters(name="fixedConstraints", boxROI=[[0, -20, -20], [1, 20, 20]])).modify(Logo)
    SingleNodeModifier(SimulationSettingsParameters(name="settings")).modify(root)
    PairNodeModifier(AttachmentConstraintParameters(name="FirstPair")).modify(Logo1, Logo2)
    PairNodeModifier(AttachmentConstraintParameters(name="SecondPair")).modify(Logo1, Logo2, holder=root)


    root.gravity=[0,0,9.81]
    Sofa.Simulation.initRoot(root)

    import Sofa.Gui

    # Find out the supported GUIs
    print ("Supported GUIs are: " + Sofa.Gui.GUIManager.ListSupportedGUI(","))
    # Launch the GUI (imgui is now by default, to use Qt please refer to the example "basic-useQtGui.py")
    Sofa.Gui.GUIManager.Init("myscene", "imgui")
    Sofa.Gui.GUIManager.createGUI(root, __file__)
    Sofa.Gui.GUIManager.SetDimension(1080, 1080)
    # Initialization of the scene will be done here
    Sofa.Gui.GUIManager.MainLoop(root)
    Sofa.Gui.GUIManager.closeGUI()
    print("GUI was closed")


