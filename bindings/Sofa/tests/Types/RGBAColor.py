# coding: utf8

import Sofa
import unittest

class Test(unittest.TestCase):
    def test_constructor_default(self):
        v0 = Sofa.Types.RGBAColor()
        self.assertEqual( v0.r(), 1 )
        self.assertEqual( v0.g(), 1 )
        self.assertEqual( v0.b(), 1 )
        self.assertEqual( v0.a(), 1 )

    def test_constructor_fromList(self):
        v0 = Sofa.Types.RGBAColor([1.0,2.0,3.0,4.0])
        self.assertEqual( v0.r(), 1.0 )
        self.assertEqual( v0.g(), 2.0 )
        self.assertEqual( v0.b(), 3.0 )
        self.assertEqual( v0.a(), 4.0 )

    def test_constructor_fromString(self):
        v0 = Sofa.Types.RGBAColor("red")
        self.assertEqual( v0.r(), 1.0 )
        self.assertEqual( v0.g(), 0.0 )
        self.assertEqual( v0.b(), 0.0 )
        self.assertEqual( v0.a(), 1.0 )

    def test_constructor_fromInvalidList(self):
        self.assertRaises(TypeError, Sofa.Types.RGBAColor, [1.0,2.0,3.0])
        self.assertRaises(TypeError, Sofa.Types.RGBAColor, [1.0,2.0,3.0,10,100])
        self.assertRaises(TypeError, Sofa.Types.RGBAColor, [1.0,2.0,"three",4.0])

    def test_lighten(self):
        color = Sofa.Types.RGBAColor(0,0,0,0)
        lightcolor = color.lighten(0.5)
        self.assertEqual( lightcolor, Sofa.Types.RGBAColor(0.5,0.5,0.5,1.0))
        
        whitecolor = color.lighten(1.0)
        self.assertEqual( whitecolor, Sofa.Types.RGBAColor(1.0,1.0,1.0,1.0) )


