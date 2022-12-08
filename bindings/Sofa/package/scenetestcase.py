import Sofa.Core
import Sofa.Simulation
import SofaRuntime
import unittest

SofaRuntime.importPlugin("SofaComponentAll")

class SceneTestCase(unittest.TestCase):
    def __init__(self, cb):
        unittest.TestCase.__init__(self, methodName="test_createScene")
        self.cb = cb
    
    def test_createScene(self):
        root = Sofa.Core.Node("root")
        self.cb(root)
        Sofa.Simulation.init(root)

    

