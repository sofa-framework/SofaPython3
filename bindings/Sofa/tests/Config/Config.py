# coding: utf8

import Sofa.Config
from Sofa.future import __enable_feature__
import unittest

class Test(unittest.TestCase):
    def test_init_feature(self):
        Sofa.Config.init("new_feature", False)
        self.assertFalse(Sofa.Config.get("new_feature"), False)

        with __enable_feature__("new_feature", True):
            self.assertEquals(Sofa.Config.get("new_feature"), True)

        self.assertFalse(Sofa.Config.get("new_feature"), False)
