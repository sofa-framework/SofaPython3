# coding: utf8

import Sofa
import unittest

class Test(unittest.TestCase):
    def test_component_creation(self):
        root = Sofa.Core.Node("rootNode")
        root.addObject("RequiredPlugin", name="SofaAllCommonComponents")

        c = Sofa.Components.MechanicalObject(root, name="dofs")
        self.assertTrue(c.name.value == "dofs")
