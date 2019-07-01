# coding: utf8

import unittest
import Sofa
import numpy as np
import SofaRuntime

class MyForceField(Sofa.Core.ForceField):
    def __init__(self, *args, **kwargs):
        kwargs["ks"] = kwargs.get("ks", 1.0)
        kwargs["kd"] = kwargs.get("kd", 0.1)
        Sofa.Core.ForceField.__init__(self, *args, **kwargs)
                        
    def init(self):
        self.initpos = self.mstate.position.array().copy()
        self.k = np.zeros((1,1))
        self.f = []
        self.d = 0.5

    def addForce(self, m, out_force, pos, vel):
        out_force += ( (self.initpos-pos) * self.ks.value  )
                 

    def addDForce(self, df, dx, kFactor, bFactor):
        return 
        
    def addKToMatrix(self, a, b):
        print(" Python::addKToMatrix: ", a, " ", b)


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
                
class Test(unittest.TestCase):
    def test_animation(self):
        node = Sofa.Core.Node("TestAnimation")
        node.addObject("RequiredPlugin", name="SofaOpenglVisual")
        node.addObject("RequiredPlugin", name="SofaSparseSolver")
        node.addObject("OglLineAxis")

        node.addObject("DefaultAnimationLoop", name="loop")
        node.addObject("EulerImplicit")
        node.addObject("CGLinearSolver", tolerance=1e-12, threshold=1e-12)
        #node.addObject("SparseLDLSolver")

        #object1.addChild( MyForceField("customFF", ks=5.0) )
        a=node.addChild( RestShapeObject( MyForceField("customFF", ks=5.0) , name="python", position=[[i*1-10.0, 0, 0] for i in range(200)] ) )
        a.mechanical.showColor = [1.0,0.0,0.0,1.0]
        b=node.addChild( RestShapeObject( CreateObject("RestShapeSpringsForceField", stiffness=5.0) , name="c++", position=[[i*0.5-1.0, 0, 0] for i in range(1)]))
        b.mechanical.showColor = [1.0,1.0,0.0,1.0]
        
        return node

def getTestsName():
    suite = unittest.TestLoader().loadTestsFromTestCase(Test)
    return [ test.id().split(".")[2] for test in suite]

def runTests():
        import sys
        suite = None
        if( len(sys.argv) == 1 ):
            suite = unittest.TestLoader().loadTestsFromTestCase(Test)
        else:
            suite = unittest.TestSuite()
            suite.addTest(Test(sys.argv[1]))
        return unittest.TextTestRunner(verbosity=1).run(suite).wasSuccessful()

def createScene(rootNode):
        rootNode.addChild(Test().test_animation())
