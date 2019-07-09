# coding: utf8

import Sofa.Core
import Sofa.Helper
import unittest
import numpy

class Test(unittest.TestCase):
    def test_STLExporter(self):
        import SofaExporter
        Sofa.Helper.msg_error("Not implemented yet")


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
