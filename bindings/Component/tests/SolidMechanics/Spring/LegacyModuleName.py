# coding: utf8

import unittest

import Sofa.Component.SolidMechanics.Spring as Spring
import Sofa.Component.SolidMechanics.FEM.Elastic as Elastic


class Test(unittest.TestCase):
    def __init__(self, a):
        unittest.TestCase.__init__(self, a)

    def test_module_path(self):
        self.assertEqual(Spring.__name__, "Sofa.Component.SolidMechanics.Spring")
        self.assertEqual(Spring.LinearSpring.__module__, "Sofa.Component.SolidMechanics.Spring")
        self.assertEqual(Spring.SpringForceField_Vec3d.__module__, "Sofa.Component.SolidMechanics.Spring")

    def test_deprecated_SofaDeformable(self):
        # The deprecated module re-exports the bindings of both SOFA modules it used to contain
        from Sofa import SofaDeformable
        self.assertIs(SofaDeformable.LinearSpring, Spring.LinearSpring)
        self.assertIs(SofaDeformable.SpringForceField_Vec3d, Spring.SpringForceField_Vec3d)
        self.assertIs(SofaDeformable.toLameParameters3D, Elastic.toLameParameters3D)


if __name__ == '__main__':
    unittest.main()
