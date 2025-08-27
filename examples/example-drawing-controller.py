import Sofa
import SofaTypes 

class DrawingExamples(Sofa.Core.Controller):
    def __init__(self, *args, **kwargs):
        # These are needed (and the normal way to override from a python class)
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        
        self.target = kwargs.get("target", None)

    def draw(self, visual_context):
        dt = visual_context.getDrawTool()
        dt.drawPoints([SofaTypes.Vec3d(-1.5,0,-1)], 5.0)
        dt.drawPoints([SofaTypes.Vec3d(-1.3,0,-1), SofaTypes.Vec3d(1.3,0,-1)], 5.0)
        dt.drawLines([SofaTypes.Vec3d(-1.3,0,-1), SofaTypes.Vec3d(1.3,0,-1)], 1.0)
        dt.drawFrames([SofaTypes.Vec3d(-1.5,0.1,-1)], [SofaTypes.Quat(0.0,0,0,1.0)], SofaTypes.Vec3d(0.1,0.1,0.1))

        if self.target is not None:
            dt.drawPoints(self.target.position, 2.0)
        
        dt.draw3DText(SofaTypes.Vec3d(-2.0,0.0,0.0), 0.5, "This is not a raptor")

def createScene(root):
    root.dt = 0.01
    root.bbox = [[-1,-1,-1],[1,1,1]]
    root.addObject('DefaultVisualManagerLoop')
    root.addObject('DefaultAnimationLoop')

    root.addObject("MeshOBJLoader", name="loader", filename="mesh/raptor_35kp.obj")

    # Add our python controller in the scene
    root.addObject( DrawingExamples(name="DrawingController2", target=root.loader) )

    
