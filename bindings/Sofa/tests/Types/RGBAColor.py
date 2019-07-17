# coding: utf8

import Sofa
import unittest

class Test(unittest.TestCase):
    def test_constructor_default(self):
        v0 = Sofa.PyTypes.RGBAColor()
        self.assertEqual( v0.r(), 0 )
        self.assertEqual( v0.g(), 0 )
        self.assertEqual( v0.b(), 0 )
        self.assertEqual( v0.a(), 0 )

    def test_constructor_fromList(self):
        v0 = Sofa.PyTypes.RGBAColor([1.0,2.0,3.0,4.0])
        self.assertEqual( v0.r(), 1.0 )
        self.assertEqual( v0.g(), 2.0 )
        self.assertEqual( v0.b(), 3.0 )
        self.assertEqual( v0.a(), 4.0 )

    def test_constructor_fromInvalidList(self):
        self.assertRaises(ValueError, Sofa.PyTypes.RGBAColor, [1.0,2.0,3.0])
        self.assertRaises(ValueError, Sofa.PyTypes.RGBAColor, [1.0,2.0,3.0,10,100])
        self.assertRaises(TypeError, Sofa.PyTypes.RGBAColor, [1.0,2.0,"three",4.0])

