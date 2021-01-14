# coding: utf8

import Sofa
import unittest

class Test(unittest.TestCase):
    def test_moduleImport(self):
        import SofaRuntime

        SofaRuntime.importPlugin('SofaSparseSolver')
