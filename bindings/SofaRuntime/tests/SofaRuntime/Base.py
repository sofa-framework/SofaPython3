# coding: utf8

import Sofa
import unittest

class Test(unittest.TestCase):
    def test_moduleImport(self):
        import SofaRuntime
        SofaRuntime.importPlugin('Sofa.Component')

    def test_getCategoriesmethod(self):
        import SofaRuntime
        SofaRuntime.importPlugin("Sofa.Component")
        t = SofaRuntime.getCategories("MeshOBJLoader")
        self.assertEquals(len(t),1)
        self.assertIsInstance(t, list)
