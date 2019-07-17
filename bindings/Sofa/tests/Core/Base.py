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

    def test_addNewData_from_parent(self):
        node = Sofa.Core.Node("a_node")
        obj1 = node.addObject("MechanicalObject", name="an_object", position=[
                              [0, 0, 0], [1, 1, 1], [2, 2, 2]])
        obj2 = node.addObject("MechanicalObject", name="another_object", position=[
                              [0, 0, 0], [1, 1, 1], [2, 2, 2]])

        obj2.addData(name="an_objectName", value="@an_object.name")

        self.assertTrue(hasattr(obj2, "an_objectName"))
        self.assertEqual(obj2.an_objectName.getParent(), obj1.name)
        self.assertEqual(obj2.an_objectName.value, "an_object")

        obj1.name = "I_Changed"
        self.assertEqual(obj2.an_objectName.value, "I_Changed")

        obj2.addData(name="an_objectName2", value=obj1.name)

        self.assertTrue(hasattr(obj2, "an_objectName2"))
        self.assertEqual(obj2.an_objectName2.getParent(), obj1.name)
        self.assertEqual(obj2.an_objectName2.value, "I_Changed")

        obj1.name = "I_Changed_Again"
        self.assertEqual(obj2.an_objectName2.value, "I_Changed_Again")

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

