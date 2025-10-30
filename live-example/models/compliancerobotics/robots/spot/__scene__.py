import Sofa
from splib.simulation.linear_solvers import addLinearSolver
from splib.simulation.ode_solvers import addImplicitODE
from splib.simulation.headers import setupDefaultHeader 

from stlib.core.basePrefab import BasePrefab
from splib3 import animation
from splib3.numerics.quat import Quat

from compliancerobotics.robots.spot import SpotRobot
from cristal.prefabs.interactivecamera import InteractiveCamera

import numpy
import math
def setupScene(self, **kwargs):
    self.addObject('VisualStyle', name="visual_style")
    self.gravity = [0, 0, -9.81]
    self.dt = 0.01

    self.add("BackgroundSetting", name="background_setting", color=[0.0,0.0,0.0,1.0])
    self.add("RequiredPlugin", name="external_plugins", pluginName=['Sofa.Component.Constraint.Projective',
                                                 'Sofa.Component.Engine.Select',
                                                 'Sofa.Component.LinearSolver.Direct',
                                                 'Sofa.Component.Mass',
                                                 'Sofa.Component.ODESolver.Backward',
                                                 'Sofa.Component.SolidMechanics.FEM.Elastic',
                                                 'Sofa.Component.StateContainer',
                                                 'Sofa.Component.Topology.Container.Grid',
                                                 'Sofa.Component.IO.Mesh',
                                                 'Sofa.Component.LinearSolver.Direct',
                                                 'Sofa.Component.ODESolver.Forward',
                                                 'Sofa.Component.Topology.Container.Dynamic',
                                                 'Sofa.Component.Visual',
                                                 'Sofa.Component.Constraint.Lagrangian.Solver'
                                                 ])

    self.addObject("FreeMotionAnimationLoop")
    self.addObject('ProjectedGaussSeidelConstraintSolver', name='constraint_solver', 
                    tolerance=1e-8, maxIterations=500, multithreading=True)
    self.addChild('modelling')
    self.addChild('simulation')
    return self 

def Simulation(name="Simulation", **kwargs):
    def ImplicitODE(parent):
        return addImplicitODE(parent)

    def LinearSolver(parent):
        return addLinearSolver(parent)

    self = Sofa.Core.Node(**({"name":name} | kwargs))
    self.apply(addImplicitODE)                           # or add or mutate or ? 
    self.apply(addLinearSolver)
    return self

def setupSimulation(self, **kwargs):
    self.apply(addImplicitODE, **kwargs)
    self.apply(addLinearSolver, **kwargs)
    self.add('GenericConstraintCorrection', name="constraint_correction", linearSolver=self.LinearSolver.linkpath)
    return self

class SpotWithGS(SpotRobot):
    @staticmethod
    def get_asset(name) -> str:
        import os
        return os.path.join(os.path.join(os.path.dirname(__file__), "assets"), name)

    def __init__(self, *args, **kwargs):
        SpotRobot.__init__(self,*args,**kwargs)
        self.setup_visual()
        self.create_visual()

    def setup_visual(self):
        self.settings.add("RequiredPlugin", name="Sofa.PointCloud")

    def create_visual(self):
        child = self.add(Sofa.Core.Node, name="visual")
        g=child.add("PointCloudContainer", name="loader", filename=SpotWithGS.get_asset("spot-cleaned.ply"))
        g=child.add("PointCloudContainer", name="geometry", filename=SpotWithGS.get_asset("spot-cleaned.ply"))
        g=child.add("PointCloudTransform", name="transform", input=child.loader.linkpath, output=child.geometry.linkpath,
            scale = [1.5,1.0,1.0])
        
class EulerToQuaternion(Sofa.Core.Controller):
    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self,*args, **kwargs)
        self.addData(name="euler_angle", value=[0.0,0.0,0.0], default=[0.0,0.0,0.0], help="", group="Input", type="Vec3d")
        self.addData(name="quaternion", value=[0.0,0.0,0.0,1.0], default=[0.0,0.0,0.0,1.0], help="", group="Output", type="Quat")        

    def onIdleEvent(self, event):
        self.quaternion = list(Quat.createFromEuler(self.euler_angle.value))

class RigidDofBuilder(Sofa.Core.Controller):
    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self,*args, **kwargs)
        self.addData(name="translation", value=[0.0,0.0,0.0], default=[0.0,0.0,0.0], help="", group="Input", type="Vec3d")        
        self.addData(name="orientation", value=[0.0,0.0,0.0,1.0], default=[0.0,0.0,0.0,1.0], help="", group="Input", type="Quat")        
        self.addData(name="rigid", value=[0.0,0.0,0.0,0.0,0.0,0.0,1.0], default=[0.0,0.0,0.0,0.0,0.0,0.0,1.0], help="", group="Output", type="Rigid3::Coord")        
    
    def onIdleEvent(self, event):
        self.rigid = self.translation.value.tolist() + self.orientation.value.tolist()

import Sofa
import json 

def set(base : Sofa.Core.Base, **kwargs):
    for k,v in kwargs.items():
        d = base.getData(k)
        if d is not None:
            d.value = v

def set_if(root, cond, **kwargs):
    for object in root.objects:
        if cond(object):
            set(object, **kwargs)

    for child in root.children:
        if cond(child):
            set(child, **kwargs)
        set_if(child, cond, **kwargs)

class IdentitySkinning(Sofa.Core.Controller):
    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.addData("indices", value=[0], default=[0], help="size", group="", type="vector<int>")
        self.container = kwargs.get("container")
        self.renderer = kwargs.get("renderer")
        self.target = kwargs.get("target")
        self.model = kwargs.get("model")
        
        self.vtx_group = dict(json.load(open(SpotRobot.get_asset("spot-vertex-group.json"))))
        print(self.vtx_group.keys())
        
        self.target.showObject=True
        self.names = {
            "universe" : "Body",
            "fl.hy" : "LegLeftFrontUp",
            "fl.kn" : "LegLeftFrontDown",
            "fr.hy" : "LegRightFrontUp",
            "fr.kn" : "LegRightFrontDown",
            "hl.hy" : "LegLeftBackUp",
            "hl.kn" : "LegLeftBackDown",
            "hr.hy" : "LegRightBackUp",
            "hr.kn" : "LegRightBackDown",            
        }
        self.names2idx = {
            "Body" : 0,
            "LegLeftFrontUp" : 1,
            "LegLeftFrontDown": 2,
            "LegRightFrontUp": 3,
            "LegRightFrontDown": 4,
            "LegLeftBackUp": 5,
            "LegLeftBackDown": 6,
            "LegRightBackUp": 7,
            "LegRightBackDown": 8            
        }
        self.references = {
        "universe" : [[0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0]],
        "fl.hy" : [[ 2.97850000e-01, 1.65945000e-01, -1.38771850e-11, -5.61779914e-11, 4.39462314e-01, -2.74843384e-11, 8.98261028e-01]],
        "fl.kn" : [[ 6.01576952e-02, 1.65945000e-01, -2.16443106e-01, -5.96543813e-11, -3.00293175e-01, 1.87805848e-11, 9.53846953e-01]],
        "fr.hy" : [[ 2.97850000e-01, -1.65945000e-01, -1.38771850e-11, 5.61779914e-11,  4.39462314e-01, 2.74843384e-11, 8.98261028e-01]],
        "fr.kn" : [[ 6.01576952e-02, -1.65945000e-01, -2.16443106e-01, 5.96543813e-11, -3.00293175e-01, -1.87805848e-11, 9.53846953e-01]],
        "hl.hy" : [[-2.97850000e-01, 1.65945000e-01, -1.38771852e-11, -5.61779920e-11,  4.39462314e-01, -2.74843387e-11, 8.98261028e-01]],
        "hl.kn" : [[-5.35542305e-01, 1.65945000e-01, -2.16443106e-01, -5.96543819e-11, -3.00293175e-01, 1.87805850e-11, 9.53846953e-01]],
        "hr.hy" : [[-2.97850000e-01, -1.65945000e-01, -1.38771852e-11, 5.61779920e-11,  4.39462314e-01, 2.74843387e-11, 8.98261028e-01]],
        "hr.kn" : [[-5.35542305e-01, -1.65945000e-01, -2.16443106e-01, 5.96543819e-11, -3.00293175e-01, -1.87805850e-11, 9.53846953e-01]] 
        }
        self.initializeFrames()
        
    def initializeFrames(self):
        splats_count = len(self.container.positions.value)
        frames_count = len(self.vtx_group.keys())
        indices = [0]*splats_count
        
        for k,v in self.vtx_group.items():
            frame_id = self.names2idx[k]
            for i in v:
                indices[i] = frame_id

        self.renderer.frameIndices = indices 
        self.target.position = [[0,0,0,0,0,0,1]]*frames_count

        with self.target.position.writeableArray() as w:
            i = 0
            for k,v in self.references.items():
                w[i:,:] = v[0]        
                i+=1

    def onAnimateEndEvent(self,event):
        with self.target.position.writeableArray() as w:
            i = 0
            for k,v in self.references.items():
                w[i:,:] = self.model.model.Frames.getChild(k).dof.position.value[0] 
                i+=1

class SpotController(Sofa.Core.Controller):
    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self,*args,**kwargs)
        self.target = kwargs.get("target")

    def onKeypressedEvent(self, event):
        def myAnimation(factor, target):
            self.target.model.getObject("jointfl.hy").value = 0.2*factor*3
            self.target.model.getObject("jointfl.kn").value = 0.5*math.sin(factor*2.0*3.14/2)

            self.target.model.getObject("jointfr.hy").value = 2.0*math.cos(factor*2.0*3.14)
            self.target.model.getObject("jointfr.kn").value = 2.0*math.sin(3+factor*1.5*3.14)-1.0

            self.target.model.getObject("jointhl.hy").value = 0.2*factor*1.5+0.3
            self.target.model.getObject("jointhl.kn").value = math.sin(factor*2.0*3.14/2)

            self.target.model.getObject("jointhr.hy").value = 0.4*factor*3
            self.target.model.getObject("jointhr.kn").value = 1.5*math.sin(factor*2.0*3.14/2)-1.0

        if event["key"] == 'P':
            set_if(self.target.model, lambda x: isinstance(x, Sofa.Core.Node) and x.name.value == "Visual",
                    activated = False)
        else: 
            animation.animate(myAnimation, {"target":self}, duration=1.0, mode="pingpong")

def setupAnimation(parent):
    return parent.addObject(animation.AnimationManager(parent))

def createScene(root):
    import SofaRuntime
    SofaRuntime.DataRepository.addLastPath("/home/dmarchal/projects/dev/sofa1/plugins/Sofa.PointCloud/examples/assets")

    root.apply(setupScene)
    root.apply(setupAnimation)

    root.add(InteractiveCamera, name="camera")

    spot = Sofa.Core.Node("spot2")
    spot.add(SpotWithGS(name="model"))
    spot.add(SpotController, name="controller", target=spot.model)
    spot.model.visual.init()
    root.modelling.add(spot)

    root.add(EulerToQuaternion, name="eulertoquat")
    root.add(RigidDofBuilder, name="rigidbuilder")
    root.rigidbuilder.orientation.setParent(root.eulertoquat.quaternion.linkpath)
    
    spot.model.visual.transform.frame = [0.05, -0.145, 0.305, 0.011913, -0.0587687, 0.198312, 0.978303]
    spot.model.visual.transform.scale = [1.20, 1, 1]

    #spot.model.visual.transform.frame.setParent(root.rigidbuilder.rigid.linkpath)

    root.add("MechanicalObject", name="renderer_frames", template="Rigid3")    
    root.add("PointCloudRenderer", name="renderer",
                                   geometry=spot.model.visual.geometry.linkpath,
                                   camera=root.camera.state.linkpath,
                                   frames=root.renderer_frames.position.linkpath
                                   )

    root.add(IdentitySkinning, name="skin", model=spot.model,
                                            target=root.renderer_frames, 
                                            renderer=root.renderer, 
                                            container=spot.model.visual.geometry)

    root.simulation.apply(setupSimulation)    
    root.simulation.add(root.modelling)
