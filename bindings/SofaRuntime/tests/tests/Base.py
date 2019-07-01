# coding: utf8

import Sofa
import unittest

class Test(unittest.TestCase):
    def test_moduleImport(self):
        import SofaRuntime

        SofaRuntime.Input.isKeyPressed(0)
        SofaRuntime.Input.isMouseButtonPressed(0)
        SofaRuntime.Input.isMouseButtonPressed(1)
        SofaRuntime.Input.isMouseButtonPressed(2)

        SofaRuntime.Input.mousePosition


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
