# coding: utf8

import Sofa
import unittest

class Test(unittest.TestCase):
    def test_component_creation(self):
        root = Sofa.Core.Node("rootNode")
        root.addObject("RequiredPlugin", name="SofaAllCommonComponents")

        c = Sofa.Components.MechanicalObject(root, name="dofs")
        self.assertTrue(c.name.value == "dofs")

def getTestsName():
    suite = unittest.TestLoader().loadTestsFromTestCase(Test)
    return [test.id().split(".")[2] for test in suite]


def runTests():
    import sys
    suite = None
    if(len(sys.argv) == 1):
        suite = unittest.TestLoader().loadTestsFromTestCase(Test)
    else:
        suite = unittest.TestSuite()
        suite.addTest(Test(sys.argv[1]))
    return unittest.TextTestRunner(verbosity=1).run(suite).wasSuccessful()


def createScene(rootNode):
    runTests()
