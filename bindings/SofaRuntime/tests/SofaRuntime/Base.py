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
        self.assertEqual(len(t),1)
        self.assertIsInstance(t, list)

    def test_findPlugin(self):
        import os
        import SofaRuntime
        path = SofaRuntime.findPlugin("Sofa.Component")
        self.assertIsInstance(path, str)
        self.assertNotEqual(path, "")
        self.assertTrue(os.path.isfile(path))

    def test_findPlugin_nonExisting(self):
        import SofaRuntime
        name = "ThisPluginDoesNotExist_SofaPython3Test"
        self.assertEqual(SofaRuntime.findPlugin(name), "")
        # Searching must not have loaded anything
        self.assertFalse(SofaRuntime.isPluginLoaded(name))

    def test_isPluginLoaded(self):
        import SofaRuntime
        SofaRuntime.importPlugin("Sofa.Component")
        self.assertTrue(SofaRuntime.isPluginLoaded("Sofa.Component"))
        self.assertFalse(SofaRuntime.isPluginLoaded("ThisPluginDoesNotExist_SofaPython3Test"))
