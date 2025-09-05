import Sofa
import SofaTypes 
import numpy
from numpy import array
from Sofa.Types import RGBAColor 


class DrawingExamples(Sofa.Core.Controller):
    def __init__(self, *args, **kwargs):
        # These are needed (and the normal way to override from a python class)
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        
        self.target = kwargs.get("target", None)
        self.mo = kwargs.get("mo", None)

    def draw(self, visual_context):
        dt = visual_context.getDrawTool()
        dt.drawPoints(array([[-1.5,-1.0,0.0]]), 5.0, RGBAColor("red"))
        dt.drawPoints([[-1.3,0,-1], [1.3,0,-1]], 10.0, RGBAColor("green"))
        dt.drawLines([[-1.3,0,-1], [1.3,0,-1]], 1.0, RGBAColor("green"))
        dt.drawFrames([[-1.5,0.1,-1]], [[0.0,0,0,1.0]], [0.1,0.1,0.1])

        if self.target is not None:
            dt.drawPoints(self.target.position, 2.0, RGBAColor("blue"))

        dt.drawText([-2.0,0.0,0.0], 0.5, "This is not a raptor", RGBAColor("white"))
        dt.drawOverlayText([10, 10], 12, "Overlay text", RGBAColor("pink"))

        dt.drawFrames(self.mo.position, [0.1,0.1,0.1])

def createScene(root):
    root.dt = 0.01
    root.bbox = [[-1,-1,-1],[1,1,1]]
    root.addObject('DefaultVisualManagerLoop')
    root.addObject('DefaultAnimationLoop')

    root.addObject("MeshOBJLoader", name="loader", filename="mesh/raptor_35kp.obj")

    root.addObject("MechanicalObject", 
                   name="mo", 
                   template="Rigid3", 
                   position=[[float(i)/10.0, 0.0,0.0, 0.0, 0.0, 0.0, 1.0] for i in range(0,10)])

    # Add our python controller in the scene
    root.addObject( DrawingExamples(name="DrawingController2", 
                                    target=root.loader,
                                    mo=root.mo) )

    
