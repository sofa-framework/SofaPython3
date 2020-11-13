# coding: utf8

import Sofa.Core
import Sofa.Components
import unittest

class Test(unittest.TestCase):
    def test_component_creation(self):
        root = Sofa.Core.Node("rootNode")

        c = Sofa.Components.MechanicalObject(root, name="dofs")
        self.assertTrue(c.name.value == "dofs")
