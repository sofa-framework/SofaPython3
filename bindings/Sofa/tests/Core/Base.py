# coding: utf8

import Sofa
import unittest


class Test(unittest.TestCase):
    def test_data_property(self):
        root = Sofa.Core.Node("rootNode")
        c = root.createObject("MechanicalObject", name="t", position=[
                              [0, 0, 0], [1, 1, 1], [2, 2, 2]])
        self.assertTrue(hasattr(c, "__data__"))
        self.assertGreater(len(c.__data__), 0)
        self.assertTrue("name" in c.__data__)
        self.assertTrue("position" in c.__data__)
        self.assertFalse(hasattr(c.__data__, "invalidEntry"))
        self.assertTrue(isinstance(c.__data__, Sofa.Core.DataDict))

    def test_addNewData_with_type(self):
        node = Sofa.Core.Node("a_node")
        obj = node.addObject("MechanicalObject", name="an_object", position=[
                             [0, 0, 0], [1, 1, 1], [2, 2, 2]])
        # Check PSDEObjectFactory to see available types
        node.addData(name="myData", value=42, type="int",
                     help="A simple integer value", group="MyAddedDataGroup")
        obj.addData(name="myData", value=42, type="int",
                    help="A simple integer value", group="MyAddedDataGroup")

        self.assertTrue(hasattr(node, "myData"))
        self.assertEqual(node.myData.value, 42)

        self.assertTrue(hasattr(obj, "myData"))
        self.assertEqual(obj.myData.value, 42)

    def test_addKeywordProtected(self):
        node = Sofa.Core.Node("a_node")
        self.assertRaises(ValueError, node.addObject, "MechanicalObject", name="children", position=[[0,0,0],[1,1,1],[2,2,2]])
        self.assertRaises(ValueError, node.addChild, "parents")
        self.assertRaises(ValueError, node.addData, name="links", type="int", value=42)

    def test_addNewData_from_parent_linkPath(self):
        root = Sofa.Core.Node('root')
        c1 = root.addObject("MechanicalObject", name="c1")
        c1.addData("data1", value="coucou", type="string")
        c1.addData("data2", value=42, type="int")
        c1.addData("data3", value=False, type="bool")
        self.assertEqual(c1.data1.value, "coucou")
        self.assertEqual(c1.data2.value, 42)
        self.assertEqual(c1.data3.value, False)

        c2 = root.addObject("MechanicalObject", name="c2")
        c2.addData("data1", value="@/c1.data1") # absolute path
        c2.addData("data2", value="@c1.data2") # relative path (local)
        c2.addData("data3", value="@./c1.data3") # relative path (local)
        self.assertEqual(c2.data1.value, c1.data1.value)
        self.assertEqual(c2.data2.value, c1.data2.value)
        self.assertEqual(c2.data3.value, c1.data3.value)

        n1 = root.addChild("n1")
        n2 = root.addChild("n2")

        c3 = n1.addObject('MechanicalObject', name='c3')
        c3.addData("data1", value="@../c1.data1") # relative path (down)
        c3.addData("data2", value="@../c2.data2") # relative path (down)
        c3.addData("data3", value="@/c2.data3") # absolute path
        self.assertEqual(c3.data1.value, c1.data1.value)
        self.assertEqual(c3.data2.value, c2.data2.value)
        self.assertEqual(c3.data3.value, c2.data3.value)

        c4 = n2.addObject('MechanicalObject', name='c4')
        c4.addData('data1', value="@/n1/c3.data1") # absolute path (chained)
        c4.addData('data2', value="@../n1/c3.data2") # relative path (down, chained)
        self.assertEqual(c4.data1.value, c3.data1.value)
        self.assertEqual(c4.data2.value, c3.data2.value)

        # Error mgmt testing
        self.assertRaises(ValueError, c4.addData, name="data3", value="@aBroken/path.value")
        self.assertRaises(ValueError, c4.addData, name="data3", value="@aBroken/path")
        self.assertRaises(ValueError, c4.addData, name="data3", value="@/aBroken/path")
        self.assertRaises(TypeError, c4.addData, name="data3", value="")
        self.assertRaises(TypeError, c4.addData, name="data3", value="/")
        self.assertRaises(TypeError, c4.addData, name="data3", value=".")
        self.assertRaises(ValueError, c4.addData, name="data3", value="@")
        self.assertRaises(ValueError, c4.addData, name="data3", value="@/")
        self.assertRaises(ValueError, c4.addData, name="data3", value="@./")
        self.assertRaises(ValueError, c4.addData, name="data3", value="@../")

        # parenting tests
        c1.data1 = "test"
        c1.data2 = 1337
        c1.data3 = False
        self.assertEqual("test", c1.data1.value)
        self.assertEqual(1337, c1.data2.value)
        self.assertEqual(False, c1.data3.value)

        self.assertEqual(c2.data1.value, c1.data1.value)
        self.assertEqual(c2.data2.value, c1.data2.value)
        self.assertEqual(c2.data3.value, c1.data3.value)


    def test_getClassName(self):
        root = Sofa.Core.Node("root")
        self.assertEqual(root.getClassName(), "DAGNode")

    def test_getTemplateName(self):
        root = Sofa.Core.Node("root")
        c = root.addObject("MechanicalObject", name="t")
        self.assertEqual(c.getTemplateName(),"Vec3d")

    def test_addExistingDataAsParentOfNewData(self):
        # TODO(@marques-bruno)
        # do a test like this:
        # obj1 = root.createObject('AComposant', aDataField="pouet")
        # obj2 = root.createobject('AnotherComposant')
        # obj2.addData(obj1.aDataField)
        # self.assertTrue(hasattr(obj2, "aDataField"))
        # self.assertEqual(obj2.an_objectName.getParent(), obj1.aDataField)
        # self.assertEqual(obj2.an_objectName.value, "pouet")

        # And another one like this:
        # aData = createAnOrphanData(name="MyData", value="pouet", type="str")
        # obj1 = root.createObject('AComposant', aDataField="pouet")
        # obj1.addData(aData)
        # self.assertTrue(hasattr(obj1, "aData"))
        # self.assertEqual(obj2.aData.getOwner(), obj1)
        # self.assertEqual(aData.getOwner(), obj1)
        # self.assertEqual(obj2.aData.value, "pouet")
        pass

