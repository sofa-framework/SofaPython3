# coding: utf8
import unittest
import Sofa
from Sofa.PyTypes import Vec3

def create_scene(rootName="root"):
    root = Sofa.Core.Node(rootName)
    root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
    return root

class Test(unittest.TestCase):
    def test_constructor(self):
        v0 = Vec3()
        v1 = Vec3([1.0,1.0,1.0])
        v2 = v0 + v1
        self.assertTrue( isinstance(v2, Vec3 ) )
        self.assertEqual( v2, v1 )

    def test_wrapAroundArray(self):
        n = create_scene()
        m = n.addObject("MechanicalObject", position=[[1.0,1.1,1.2],[2.0,2.1,2.2],[3.0,3.1,3.2]])
        self.assertRaises(AttributeError, Vec3, m.position.array())

    def test_wrapInvalidSize(self):
        n = create_scene()
        m = n.addObject("MechanicalObject")
        def d():
            return Vec3(m.showColor)
        self.assertRaises(AttributeError, d)


    def test_wrapAroundSingleField(self):
        n = create_scene()
        m = n.addObject("MechanicalObject", translation=[1.0,0.0,0.0])
        c = Vec3(m.translation)
        self.assertEqual(c.tolist(), [1.0,0.0,0.0])

        c2 = Vec3(m.translation.value)
        self.assertEqual(c2.tolist(), [1.0,0.0,0.0])

        c3 = Vec3(m.translation.array())
        self.assertEqual(c3.tolist(), [1.0,0.0,0.0])

