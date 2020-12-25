# coding: utf8

import unittest
from Sofa.SofaDeformable import LinearSpring


class Test(unittest.TestCase):
    def __init__(self, a):
        unittest.TestCase.__init__(self, a)

    def test_Creation(self):
        spring = LinearSpring(
            index1=0,
            index2=1,
            springStiffness=100,
            dampingFactor=5,
            restLength=5,
            elongationOnly=False,
            enabled=True,
        )
        self.assertEqual(spring.index1, 0)
        self.assertEqual(spring.index2, 1)
        self.assertEqual(spring.springStiffness, 100)
        self.assertEqual(spring.dampingFactor, 5)
        self.assertEqual(spring.restLength, 5)
        self.assertFalse(spring.elongationOnly)
        self.assertTrue(spring.enabled)

    def test_SetValues(self):
        spring = LinearSpring(
            index1=0,
            index2=1,
            springStiffness=100,
            dampingFactor=5,
            restLength=5,
            elongationOnly=False,
            enabled=True,
        )
        spring.index1 = 1
        spring.index2 = 0
        spring.springStiffness = 200
        spring.dampingFactor = 10
        spring.restLength = 10
        spring.elongationOnly = True
        spring.enabled = False
        self.assertEqual(spring.index1, 1)
        self.assertEqual(spring.index2, 0)
        self.assertEqual(spring.springStiffness, 200)
        self.assertEqual(spring.dampingFactor, 10)
        self.assertEqual(spring.restLength, 10)
        self.assertTrue(spring.elongationOnly)
        self.assertFalse(spring.enabled)
