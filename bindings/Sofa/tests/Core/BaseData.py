# coding: utf8

import unittest
import numpy
import Sofa
import Sofa.Core
from Sofa.PyTypes import RGBAColor

class Test(unittest.TestCase):    
    def test_typeNameDouble(self):
        root = Sofa.Core.Node("rootNode")
        c = root.addObject("MechanicalObject", name="t", position=[
                           [0, 0, 0], [1, 1, 1], [2, 2, 2]])
        self.assertEqual(c.position.typeName(), "vector<Vec3d>")
        self.assertEqual(c.showColor.typeName(), "Vec4f")

    def test_typeName(self):
        root = Sofa.Core.Node("rootNode")
        c = root.addObject("MechanicalObject", name="t", position=[
                           [0, 0, 0], [1, 1, 1], [2, 2, 2]])
        self.assertEqual(c.position.typeName(), "vector<Vec3d>")

    # @unittest.skip  # no reason needed
    def test_ValidDataAccess(self):
        root = Sofa.Core.Node("rootNode")
        c = root.addObject("MechanicalObject", name="t", position=[
                           [0, 0, 0], [1, 1, 1], [2, 2, 2]])
        self.assertTrue(c.position is not None)

    def test_DataContainerDimmensions(self):
        root = Sofa.Core.Node("rootNode")
        c = root.addObject("MechanicalObject", name="t", position=[
                               [0, 0, 0], [1, 1, 1], [2, 2, 2], [3, 3, 3]])
        self.assertEqual(len(c.position), 4)
        self.assertEqual(c.position.ndim, 2)
        self.assertEqual(len(c.position.shape), c.position.ndim)
        self.assertEqual(c.position.shape[0], 4)
        self.assertEqual(c.position.shape[1], 3)

    # @unittest.skip  # no reason needed
    def test_InvalidDataAccess(self):
        root = Sofa.Core.Node("rootNode")
        self.assertRaises(AttributeError, getattr, root, "invalidData")

    # @unittest.skip  # no reason needed
    def test_DataAsArray2D(self):
        root = Sofa.Core.Node("rootNode")
        v = [[0, 0, 0], [1, 1, 1], [2, 2, 2], [3, 3, 3]]
        c = root.addObject("MechanicalObject", name="t", position=v)
        self.assertEqual(len(c.position), 4)
        self.assertEqual(len(c.position.value), 4)
        self.assertSequenceEqual(list(c.position.value[0]), v[0])
        self.assertSequenceEqual(list(c.position.value[1]), v[1])
        self.assertSequenceEqual(list(c.position.value[2]), v[2])
        self.assertSequenceEqual(list(c.position.value[3]), v[3])

    # @unittest.skip  # no reason needed
    def test_DataArray2DOperationInPlace(self):
        root = Sofa.Core.Node("rootNode")
        v = numpy.array([[0, 0, 0], [1, 1, 1], [2, 2, 2], [3, 3, 3]])
        c = root.addObject("MechanicalObject", name="t", position=v.tolist())
        c.position.value *= 2.0
        numpy.testing.assert_array_equal(c.position.array(), v*2.0)
        c.position.value += 3.0
        numpy.testing.assert_array_equal(c.position.array(), (v*2.0)+3.0)

    # @unittest.skip  # no reason needed
    def test_DataArray2DSetFromList(self):
        v = [[0, 0, 0], [1, 1, 1], [2, 2, 2], [3, 3, 3]]
        root = Sofa.Core.Node("rootNode")
        c = root.addObject("MechanicalObject", name="t", position=v)
        c.position = [[1, 1, 1], [2, 2, 2], [3, 3, 3], [4, 4, 4]]
        numpy.testing.assert_array_equal(c.position.array(), [[1.0, 1.0, 1.0], [
                                         2.0, 2.0, 2.0], [3.0, 3.0, 3.0], [4.0, 4.0, 4.0]])

    # @unittest.skip  # no reason needed
    def test_DataArray2DResizeFromArray(self):
        v = [[0, 0, 0], [1, 1, 1], [2, 2, 2], [3, 3, 3]]
        root = Sofa.Core.Node("rootNode")
        c = root.addObject("MechanicalObject", name="t", position=v)
        zeros = numpy.zeros((100, 3), dtype=numpy.float64)
        c.position.value = zeros
        numpy.testing.assert_array_equal(c.position.array(), zeros)

    # @unittest.skip  # no reason needed
    def test_DataArray2DInvalidResizeFromArray(self):
        v = [[0, 0, 0], [1, 1, 1], [2, 2, 2], [3, 3, 3]]
        root = Sofa.Core.Node("rootNode")
        c = root.addObject("MechanicalObject", name="t", position=v)
        zeros = numpy.zeros((4, 100), dtype=numpy.float64)

        def d():
            c.position = zeros
        self.assertRaises(IndexError, d)

    # @unittest.skip  # no reason needed
    def test_DataArray2DSetFromArray(self):
        v = [[0, 0, 0], [1, 1, 1], [2, 2, 2], [3, 3, 3], [4, 4, 4]]
        root = Sofa.Core.Node("rootNode")
        c = root.addObject("MechanicalObject", name="t", position=v)

        zeros = numpy.zeros((500, 3), dtype=numpy.float64)
        c.position.value = zeros
        numpy.testing.assert_array_equal(c.position.array(), zeros)

        ones = numpy.ones((1000, 3), dtype=numpy.float32)
        c.position.value = ones
        numpy.testing.assert_array_equal(c.position.array(), ones)

        zeros = numpy.zeros((500, 3), dtype=numpy.float32)
        c.position.value = zeros
        numpy.testing.assert_array_equal(c.position.array(), zeros)

    @unittest.skip  # no reason needed
    def test_DataArray2DElementWiseOperation(self):
        root = Sofa.Core.Node("rootNode")
        m = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
        v = [[0, 0, 0], [1, 1, 1], [2, 2, 2], [3, 3, 3]]
        c = root.addObject("MechanicalObject", name="t", position=v)
        c.position.value *= c.position.value

    def test_DataArrayCreateFromNumpy(self):
        root = Sofa.Core.Node("rootNode")
        v = numpy.array([[0, 0, 0], [1, 1, 1], [2, 2, 2], [3, 3, 3]])
        c = root.addObject("MechanicalObject", name="t", position=v)

    def test_UnknowAttribute(self):
        """ Access a non-existing attribute of a data field so this should trigger AttributeError"""
        root = Sofa.Core.Node("root")  # < Create a new node
        # < Create a new object
        c = root.addObject("MechanicalObject", name="t")
        p = c.position.value  # < Retrive its position

        # < Check that accessing an invalid data field rise an attribute error and nothing
        # < else.
        self.assertRaises(AttributeError, (lambda aa: aa.unvalid), p)

    # @unittest.skip  # no reason needed

    def test_DataArray2DOperation(self):
        root = Sofa.Core.Node("rootNode")
        v = numpy.array([[0, 0, 0], [1, 1, 1], [2, 2, 2], [3, 3, 3]])
        c = root.addObject("MechanicalObject", name="t", position=v.tolist())
        c2 = c.position.value * 2.0

        numpy.testing.assert_array_equal(c.position.array(), v)
        numpy.testing.assert_array_equal(c2, v*2.0)

        c2 = c.position.value + 2.0
        numpy.testing.assert_array_equal(c.position.array(), v)
        numpy.testing.assert_array_equal(c2, v+2.0)

    # @unittest.skip  # no reason needed
    def test_DataAsArray1D(self):
        root = Sofa.Core.Node("rootNode")
        v = [[0, 0, 0], [1, 1, 1], [2, 2, 2]]
        c = root.addObject("MechanicalObject", name="t", position=v)
        self.assertEqual(len(c.showColor.value), 4)
        self.assertTrue(isinstance(c.showColor, Sofa.Core.DataContainer))

    # @unittest.skip  # no reason needed
    def test_DataWrapper1D(self):
        root = Sofa.Core.Node("rootNode")
        v = [[0, 0, 0], [1, 1, 1], [2, 2, 2]]
        root.addObject("MechanicalObject", name="obj", position=v)

        c = RGBAColor(root.obj.showColor)
        self.assertEqual(c.r(), 1.0)

        with root.obj.showColor.writeableArray(RGBAColor) as color:
            color[0] = 2.0

        self.assertEqual(color.r(), 2.0)

        def t(c):
            c[0] = 1.0
        self.assertRaises(ValueError, (lambda c: t(c)), color)

    def test_DataAsContainerNumpyArray(self):
        root = Sofa.Core.Node("rootNode")
        v = numpy.array([[0, 0, 0], [1, 1, 1], [2, 2, 2], [3, 3, 3]])
        c = root.addObject("MechanicalObject", name="t", position=v.tolist())

        with c.position.writeableArray() as wa:
            self.assertEqual(wa.shape, (4, 3))
            self.assertEqual(wa[0, 0], 0.0)
            self.assertEqual(wa[1, 1], 1.0)
            self.assertEqual(wa[2, 2], 2.0)
            numpy.testing.assert_array_equal(c.position.array(), v)

    def test_DataAsContainerNumpyArrayRepeat(self):
        root = Sofa.Core.Node("rootNode")
        v = numpy.array([[0, 0, 0], [1, 1, 1], [2, 2, 2], [3, 3, 3]])
        c = root.addObject("MechanicalObject", name="t", position=v.tolist())

        with c.position.writeableArray() as wa:
            wa *= 2.0
            self.assertEqual(wa.shape, (4, 3))
            self.assertEqual(wa[0, 0], 0.0)
            self.assertEqual(wa[1, 1], 2.0)
            self.assertEqual(wa[2, 2], 4.0)
            numpy.testing.assert_array_equal(wa, v*2.0)

        # Checks that the data are correctly changed in the writeable block
        numpy.testing.assert_array_equal(c.position.array(), v*2.0)

        # Checks that trying to access wa object in write correctly raise
        # an error.
        def t(c):
            c[0, 0] = 1.0
        self.assertRaises(ValueError, (lambda c: t(c)), wa)

        # Checks that the previously defined blocks is correctly re-created.
        with c.position.writeableArray() as wa:
            wa *= 2.0
            self.assertEqual(wa.shape, (4, 3))
            self.assertEqual(wa[0, 0], 0.0)
            self.assertEqual(wa[1, 1], 4.0)
            self.assertEqual(wa[2, 2], 8.0)
            numpy.testing.assert_array_equal(wa, v*4.0)


def getTestsName():
    suite = unittest.TestLoader().loadTestsFromTestCase(Test)
    return [test.id().split(".")[2] for test in suite]


def runTests():
    import sys
    suite = None
    if(len(sys.argv) == 1):
        suite = unittest.TestLoader().loadTestsFromTestCase(Test)
    else:
        suite = unittest.TestSuite()
        suite.addTest(Test(sys.argv[1]))
    return unittest.TextTestRunner(verbosity=1).run(suite).wasSuccessful()


def createScene(rootNode):
    runTests()
