# coding: utf8

import Sofa.Lifetime
from Sofa.Lifetime import __feature__
import unittest

class Test(unittest.TestCase):
    def test_init_feature(self):
        Sofa.Lifetime.init("new_feature", False)
        self.assertFalse(Sofa.Lifetimeg.get("new_feature"), False)

        with __feature__("new_feature", True):
            self.assertEquals(Sofa.Lifetime.get("new_feature"), True)

        self.assertFalse(Sofa.Lifetime.get("new_feature"), False)
