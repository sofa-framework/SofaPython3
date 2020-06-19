import unittest
import Sofa
import Sofa.Core
import numpy as np

class MyForceField(Sofa.Core.ForceField):
    def __init__(self, *args, **kwargs):
        kwargs["ks"] = kwargs.get("ks", 1.0)
        kwargs["kd"] = kwargs.get("kd", 0.1)
        Sofa.Core.ForceField.__init__(self, *args, **kwargs)
                        
    def init(self):
        self.initpos = self.mstate.position.array().copy()
        
    def addForce(self, m, out_force, pos, vel):
        out_force += ((self.initpos-pos) * self.ks.value)
        
    def addDForce(self, df, dx, kFactor, bFactor):
        df -= dx * self.ks.value * kFactor
        
class CreateObject(object):
        def __init__(self, *args, **kwargs):
                self.args = args
                self.kwargs = kwargs

def RestShapeObject(impl, name="unnamed", position=[]):
        node = Sofa.Core.Node(name)
        c = node.addObject("MechanicalObject", name="mechanical", position=position)
        c.showObject = True
        c.drawMode = 1

        m = node.addObject("UniformMass", name="mass", vertexMass=0.1)
        
        if isinstance(impl, CreateObject): 
                node.createObject(*impl.args, **impl.kwargs)        
        else:        
                d = node.addObject(impl)
        return node
 
def createScene(node):
        node.addObject("DefaultAnimationLoop", name="loop")
        node.addObject("EulerImplicit")
        node.addObject("CGLinearSolver", tolerance=1e-12, threshold=1e-12)
        
        a=node.addChild( RestShapeObject( MyForceField("customFF", ks=5.0) , name="python", position=[[i-10.0, 0, 0] for i in range(100)] ) )
        a.mechanical.showColor = [1.0,0.0,0.0,1.0]

        
######################################### TESTS ####################################################
## In the following is the code used to consider this example as a test.
####################################################################################################
class Test(unittest.TestCase):
    def test_example(self):
            createScene(Sofa.Core.Node("root"))
