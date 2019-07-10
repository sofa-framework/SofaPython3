import Sofa
import numpy as np

def unsafe_multiply(a, b):
        if isinstance(b, Sofa.Core.Data):
                return np.multiply(a, b.value, casting="unsafe")
        return np.multiply(a, b, casting="unsafe")
                      
np.set_numeric_ops(multiply=unsafe_multiply)

class MyController(Sofa.BaseController):
    def __init__(self, *args, **kwargs):
        Sofa.BaseController.__init__(self, *args, *kwargs)
                
    def onEvent(self, value):
        print("EVEnt "+value)     

class MyForceField(Sofa.ForceField):
    def __init__(self, *args, **kwargs):
        kwargs["ks"] = kwargs.get("ks", 1.0)
        kwargs["kd"] = kwargs.get("kd", 2.0)
        Sofa.ForceField.__init__(self, *args, **kwargs)
                        
    def init(self):
        self.initpos = self.mstate.position.array().copy()
        
    def addForce(self, m, out_force, pos, vel):
        out_force += ((self.initpos-pos) * self.ks)
        
    def addDForce(self, out_df, dx, kFactor, bFactor):
        out_df -= dx * self.kd * kFactor
        
def RestShapeObject(name="unnamed", position=[]):
        node = Sofa.Node(name)
        c = node.addObject("MechanicalObject", name="mechanical", position=position)
        c.showObject = True
        c.drawMode = 1
        m = node.addObject("UniformMass", name="mass", vertexMass=0.1)        
        return node
 
def createScene(node):
        node.addObject("OglLineAxis")
        node.addObject("RequiredPlugin", name="SofaSparseSolver")
        node.addObject("DefaultAnimationLoop", name="loop")
        node.addObject("EulerImplicit")
        node.addObject("CGLinearSolver", tolerance=1e-12, threshold=1e-12)
        
        a=node.addChild( RestShapeObject( 
                name="python", position=[[i-10.0, 0, 0] for i in range(100)] ) )
        a.mechanical.showColor = [1.0,0.0,0.0,1.0]
        a.addObject( MyForceField("customFF", ks=5.0) )
        a.addObject( MyController() )
        return node

