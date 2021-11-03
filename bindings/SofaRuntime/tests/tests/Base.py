# coding: utf8

import Sofa
import unittest

class Test(unittest.TestCase):
    def test_moduleImport(self):
        import SofaRuntime

        SofaRuntime.importPlugin('SofaComponentAll')

    def test_getcategoriesmethode(self):
        import SofaRuntime
        SofaRuntime.importPlugin("SofaComponentAll")
        t = SofaRuntime.getCategories("MeshObjLoader")
        self.assertEquals(len(t),1)
        self.assertIsInstance(t, list)