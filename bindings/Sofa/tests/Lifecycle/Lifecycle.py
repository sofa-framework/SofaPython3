# coding: utf8

import Sofa
from Sofa.Lifecycle import __new_feature__
import unittest

class Test(unittest.TestCase):
    def test_init_feature(self):
        Sofa.Lifecycle.register_feature("new_feature", False)
        self.assertFalse(Sofa.Lifecycle.get_feature_status("new_feature"), False)

        with __new_feature__("new_feature", True):
            self.assertEquals(Sofa.Lifecycle.get_feature_status("new_feature"), True)

        self.assertFalse(Sofa.Lifecycle.get_feature_status("new_feature"), False)
