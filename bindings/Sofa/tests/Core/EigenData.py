# coding: utf8

import unittest
import numpy
import Sofa


def create_scene(rootName="root"):
    return Sofa.Core.Node(rootName)


class Test(unittest.TestCase):

    def test_read_dynamicFloat(self):
        root = create_scene()
        c = root.addObject("Binding_BaseObject_MockComponent", name="c")
        m = c.matrixXf.value
        self.assertIsInstance(m, numpy.ndarray)
        self.assertEqual(m.shape, (2, 2))
        self.assertEqual(m.dtype, numpy.dtype(numpy.float32))
        numpy.testing.assert_array_equal(
            m, numpy.ones((2, 2), dtype=numpy.float32))
        self.assertFalse(m.flags.writeable)

    def test_read_dynamicDouble(self):
        root = create_scene()
        c = root.addObject("Binding_BaseObject_MockComponent", name="c")
        m = c.matrixXd.value
        self.assertIsInstance(m, numpy.ndarray)
        self.assertEqual(m.shape, (2, 3))
        self.assertEqual(m.dtype, numpy.dtype(numpy.float64))
        numpy.testing.assert_array_equal(m, 2.0 * numpy.ones((2, 3)))
        self.assertFalse(m.flags.writeable)

    def test_read_fixedSize(self):
        root = create_scene()
        c = root.addObject("Binding_BaseObject_MockComponent", name="c")
        m = c.matrix3d.value
        self.assertIsInstance(m, numpy.ndarray)
        self.assertEqual(m.shape, (3, 3))
        self.assertEqual(m.dtype, numpy.dtype(numpy.float64))
        numpy.testing.assert_array_equal(m, numpy.identity(3))

    def test_read_int(self):
        root = create_scene()
        c = root.addObject("Binding_BaseObject_MockComponent", name="c")
        m = c.matrixXi.value
        self.assertIsInstance(m, numpy.ndarray)
        self.assertEqual(m.shape, (2, 2))
        self.assertEqual(m.dtype, numpy.dtype(numpy.int32))
        numpy.testing.assert_array_equal(
            m, 7 * numpy.ones((2, 2), dtype=numpy.int32))

    def test_zeroCopy(self):
        # Two successive accesses must alias the same C++ storage:
        # .value exposes a view, not a copy.
        root = create_scene()
        c = root.addObject("Binding_BaseObject_MockComponent", name="c")
        v1 = c.matrixXd.value
        v2 = c.matrixXd.value
        self.assertTrue(numpy.shares_memory(v1, v2))

    def test_viewIsLive(self):
        # A view is not a snapshot: an in-place modification of the data
        # (here through the string read() path on a fixed-size matrix)
        # is visible through a previously obtained view.
        root = create_scene()
        c = root.addObject("Binding_BaseObject_MockComponent", name="c")
        v = c.matrix3d.value
        numpy.testing.assert_array_equal(v, numpy.identity(3))
        c.matrix3d = "3 3 1 2 3 4 5 6 7 8 9"
        numpy.testing.assert_array_equal(v, numpy.arange(1, 10).reshape(3, 3))

    def test_write_fromArray(self):
        root = create_scene()
        c = root.addObject("Binding_BaseObject_MockComponent", name="c")
        # a dynamic matrix is resized to the array shape
        c.matrixXd = numpy.arange(6, dtype=numpy.float64).reshape(3, 2)
        m = c.matrixXd.value
        self.assertEqual(m.shape, (3, 2))
        numpy.testing.assert_array_equal(m, numpy.arange(6).reshape(3, 2))
        # scalar type conversion: float64 array into a float32 matrix
        c.matrixXf = numpy.ones((4, 5), dtype=numpy.float64)
        self.assertEqual(c.matrixXf.value.shape, (4, 5))
        self.assertEqual(c.matrixXf.value.dtype, numpy.dtype(numpy.float32))
        # int matrix from an int64 array
        c.matrixXi = numpy.array([[1, 2], [3, 4]], dtype=numpy.int64)
        self.assertEqual(c.matrixXi.value.dtype, numpy.dtype(numpy.int32))
        numpy.testing.assert_array_equal(c.matrixXi.value, [[1, 2], [3, 4]])

    def test_write_fromList(self):
        root = create_scene()
        c = root.addObject("Binding_BaseObject_MockComponent", name="c")
        c.matrixXd = [[1.0, 2.0], [3.0, 4.0]]
        numpy.testing.assert_array_equal(
            c.matrixXd.value, [[1.0, 2.0], [3.0, 4.0]])

    def test_write_fixedSizeShapeChecked(self):
        root = create_scene()
        c = root.addObject("Binding_BaseObject_MockComponent", name="c")
        c.matrix3d = numpy.arange(9, dtype=numpy.float64).reshape(3, 3)
        numpy.testing.assert_array_equal(
            c.matrix3d.value, numpy.arange(9).reshape(3, 3))
        # a wrong shape is rejected for a fixed-size matrix
        with self.assertRaises(Exception):
            c.matrix3d = numpy.ones((2, 2))

    def test_write_fromStringStillWorks(self):
        root = create_scene()
        c = root.addObject("Binding_BaseObject_MockComponent", name="c")
        c.matrixXd = "2 2 1 2 3 4"
        m = c.matrixXd.value
        self.assertEqual(m.shape, (2, 2))
        numpy.testing.assert_array_equal(m, [[1.0, 2.0], [3.0, 4.0]])
