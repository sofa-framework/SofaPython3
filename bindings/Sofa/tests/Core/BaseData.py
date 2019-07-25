# coding: utf8

import unittest
import numpy
import Sofa
import Sofa.Core
from Sofa.PyTypes import RGBAColor
import numpy as np
import io
from contextlib import redirect_stdout

#print("DIR: ", dir(Sofa))


class NpArrayTestController(Sofa.Core.Controller):
    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)

        self.addData(name="vector_Vec3_1entry",
                     type="vector<Vec3d>", value=[[1, 2, 3]])
        self.addData(name="vector_Vec3_3entries", type="vector<Vec3d>", value=[
                     [1, 2, 3], [4, 5, 6], [7, 8, 9]])

        self.addData(name="vector_int_1entry", type="vector<int>", value=[1])
        self.addData(name="vector_int_3entries",
                     type="vector<int>", value=[1, 2, 3])

        self.addData(name="vector_scalar_1entry",
                     type="vector<double>", value=[1.0])
        self.addData(name="vector_scalar_3entries",
                     type="vector<double>", value=[1.0, 2.0, 3.0])

        self.addData(name="vector_text_1entry",
                     type="vector<string>", value=["v1"])
        self.addData(name="vector_text_3entries",
                     type="vector<string>", value=["v1", "v2", "v3"])

    def testLen(self, test):
        test.assertEqual(len(self.vector_Vec3_1entry), 1)
        test.assertEqual(len(self.vector_Vec3_3entries), 3)

        test.assertEqual(len(self.vector_int_1entry), 1)
        test.assertEqual(len(self.vector_int_3entries), 3)

        test.assertEqual(len(self.vector_scalar_1entry), 1)
        test.assertEqual(len(self.vector_scalar_3entries), 3)

        test.assertEqual(len(self.vector_text_1entry), 1)
        test.assertEqual(len(self.vector_text_3entries), 3)

    def testSize(self, test):
        test.assertEqual(self.vector_Vec3_1entry.size, 3)
        test.assertEqual(self.vector_Vec3_3entries.size, 9)

        test.assertEqual(self.vector_int_1entry.size, 1)
        test.assertEqual(self.vector_int_3entries.size, 3)

        test.assertEqual(self.vector_scalar_1entry.size, 1)
        test.assertEqual(self.vector_scalar_3entries.size, 3)

    def testShape(self, test):
        test.assertEqual(self.vector_Vec3_1entry.shape, (1, 3))
        test.assertEqual(self.vector_Vec3_3entries.shape, (3, 3))

        test.assertEqual(self.vector_int_1entry.shape, (1,))
        test.assertEqual(self.vector_int_3entries.shape, (3,))

        test.assertEqual(self.vector_scalar_1entry.shape, (1,))
        test.assertEqual(self.vector_scalar_3entries.shape, (3,))

    def testNDim(self, test):
        test.assertEqual(self.vector_Vec3_1entry.ndim, 2)
        test.assertEqual(self.vector_Vec3_3entries.ndim, 2)

        test.assertEqual(self.vector_int_1entry.ndim, 1)
        test.assertEqual(self.vector_int_3entries.ndim, 1)

        test.assertEqual(self.vector_scalar_1entry.ndim, 1)
        test.assertEqual(self.vector_scalar_3entries.ndim, 1)

    def testValue(self, test):
        test.assertEqual(str(self.vector_Vec3_1entry.array()),
                         str(np.array([[1.0, 2.0, 3.0]])))
        test.assertEqual(str(self.vector_Vec3_3entries.array()),
                         str(np.array([[1.0, 2.0, 3.0], [4.0, 5.0, 6.0], [7.0, 8.0, 9.0]])))

        test.assertEqual(str(self.vector_int_1entry.array()), str(np.array([1])))
        test.assertEqual(str(self.vector_int_3entries.array()), str(np.array([1, 2, 3])))

        test.assertEqual(str(self.vector_scalar_1entry.array()), str(np.array([1.0])))
        test.assertEqual(str(self.vector_scalar_3entries.array()),
                         str(np.array([1.0, 2.0, 3.0])))

        test.assertEqual(repr(self.vector_text_1entry), repr(["v1"]))
        test.assertEqual(repr(self.vector_text_3entries),
                         repr(["v1", "v2", "v3"]))


class Test(unittest.TestCase):
    def test_typeName(self):
        root = Sofa.Core.Node("rootNode")
        c = root.addObject("MechanicalObject", name="t", position=[
                           [0, 0, 0], [1, 1, 1], [2, 2, 2]])
        self.assertEqual(c.position.typeName(), "vector<Vec3d>")
        self.assertEqual(c.showColor.typeName(), "RGBAColor")

    # @unittest.skip  # no reason needed
    def test_ValidDataAccess(self):
        root = Sofa.Core.Node("rootNode")
        c = root.addObject("MechanicalObject", name="t", position=[
                           [0, 0, 0], [1, 1, 1], [2, 2, 2]])
        self.assertTrue(c.position is not None)

    # @unittest.skip  # no reason needed
    def test_InvalidDataAccess(self):
        root = Sofa.Core.Node("rootNode")
        self.assertRaises(AttributeError, getattr, root, "invalidData")

    # @unittest.skip  # no reason needed
    def test_DataAsArray2D(self):
        root = Sofa.Core.Node("rootNode")
        v = [[0, 0, 0], [1, 1, 1], [2, 2, 2]]
        c = root.addObject("MechanicalObject", name="t", position=v)
        self.assertEqual(len(c.position.value), 3)
        self.assertSequenceEqual(list(c.position.value[0]), v[0])
        self.assertSequenceEqual(list(c.position.value[1]), v[1])
        self.assertSequenceEqual(list(c.position.value[2]), v[2])

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
        c = root.addObject("MechanicalObject", name="t",
                           position=v, showColor=[0.42, 0.1, 0.9, 1.0])
        self.assertEqual(len(c.showColor.value), 4)
        self.assertAlmostEqual(c.showColor.value[0], 0.42)

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

    def test_DataAsContainerNumpyArray_(self):
        root = Sofa.Core.Node("rootNode")
        v = numpy.array([[0, 0, 0], [1, 1, 1], [2, 2, 2], [3, 3, 3]])
        v2 = numpy.array([0, 1, 2, 3, 4, 5])
        c = root.addObject("MechanicalObject", name="t", position=v.tolist())
        c2 = root.addObject("BoxROI", name="c2", indices=[0, 1, 2, 3, 4, 5])

        numpy.testing.assert_array_equal(c2.indices.array(), v2)
        numpy.testing.assert_array_equal(c2.indices.value, [0, 1, 2, 3, 4, 5])

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

    def test_DataString(self):
        n = Sofa.Core.Node("rootNode")
        self.assertTrue(isinstance(n.name, Sofa.Core.DataString))
        self.assertEqual(n.name.value, "rootNode")
        self.assertEqual(len(n.name), 8)

    def test_DataAsContainerNumpyArray(self):
        n = Sofa.Core.Node("rootNode")
        c = n.addObject(NpArrayTestController(name="c"))

        c.testLen(self)
        c.testSize(self)
        c.testShape(self)
        c.testNDim(self)
        c.testValue(self)

    def test_name(self):
        root = Sofa.Core.Node("root")
        root.addData("aField", 1.0 , "help message","theDataGroup", "float")
        data = root.getData("aField")
        self.assertEqual(data.name,"aField")
        data.name="aNewField"
        self.assertEqual(data.name,"aNewField")

    def test_getValueString(self):
        root = Sofa.Core.Node("root")
        c = root.addObject("MechanicalObject", name="t", position=[[0,1,0]])
        self.assertEqual(c.position.getValueString(),"0 1 0")

    def test_getValueTypeString(self):
        root = Sofa.Core.Node("root")
        c = root.addObject("MechanicalObject", name="t", position=[[0,1,0]])
        self.assertEqual(c.position.getValueTypeString(),"vector<Vec3d>")

    def test_isRequired(self):
        root = Sofa.Core.Node("root")
        self.assertFalse(root.name.isRequired())

    def test_Persistent(self):
        root = Sofa.Core.Node("root")
        root.addData("aField", 1.0 , "help message","theDataGroup", "float")
        data = root.getData("aField")
        self.assertTrue(data.isPersistent())
        data.setPersistent(False)
        self.assertFalse(data.isPersistent())

    def test_Parent(self):
        root = Sofa.Core.Node("root")
        root.addData("aField", 1.0 , "help message","theDataGroup", "float")
        root.addData("aFieldParent", 1.0 , "help message","theDataGroup", "float")
        data = root.getData("aField")
        dataParent = root.getData("aFieldParent")
        self.assertFalse(data.hasParent())
        data.setParent(dataParent, "@/dataParent/data")
        self.assertTrue(data.hasParent())
        self.assertEqual(data.getParent().name,"aFieldParent")

    def test_getLinkPath(self):
        root = Sofa.Core.Node("root")
        root.addData("aField", 1.0 , "help message","theDataGroup", "float")
        root.addData("aFieldParent", 1.0 , "help message","theDataGroup", "float")
        data = root.getData("aField")
        dataParent = root.getData("aFieldParent")
        data.setParent(dataParent, "@/dataParent/data")
        self.assertEqual(data.getLinkPath(), "@/dataParent/data")
        self.assertEqual(dataParent.getLinkPath(),"")
        self.assertEqual(data.getAsACreateObjectParameter(), "@/dataParent/data")
        self.assertEqual(dataParent.getAsACreateObjectParameter(),"")

    def test_read(self):
        root = Sofa.Core.Node("root")
        root.addData("aField", 1.0 , "help message","theDataGroup", "float")
        data = root.getData("aField")
        self.assertEqual(data.value,1.0)
        self.assertTrue(data.read("3.0"))
        self.assertEqual(data.value,3.0)
        self.assertFalse(data.read("test"))

    def test_Dirty(self):
        root = Sofa.Core.Node("root")
        root.addData("aField", 1.0 , "help message","theDataGroup", "float")
        root.addData("aFieldParent", 1.0 , "help message","theDataGroup", "float")
        data = root.getData("aField")
        dataParent = root.getData("aFieldParent")
        data.setParent(dataParent, "@/dataParent/data")
        dataParent.read("3.0")
        self.assertFalse(dataParent.isDirty())
        self.assertTrue(data.isDirty())
        data.updateIfDirty()
        self.assertFalse(data.isDirty())

    def test_readOnly(self):
        root = Sofa.Core.Node("root")
        root.addData("aField", 1.0 , "help message","theDataGroup", "float")
        data = root.getData("aField")
        self.assertFalse(data.isReadOnly())
        data.setReadOnly(True)
        self.assertTrue(data.isReadOnly())
