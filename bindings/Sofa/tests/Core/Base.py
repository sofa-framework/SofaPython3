# coding: utf8

import Sofa
import unittest

def create_scene(rootName="root"):
    root = Sofa.Core.Node(rootName)
    root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
    return root

class Test(unittest.TestCase):
    def test_data_property(self):
        root = create_scene("rootNode")
        c = root.addObject("MechanicalObject", name="t", position=[
                              [0, 0, 0], [1, 1, 1], [2, 2, 2]])
        self.assertTrue(hasattr(c, "__data__"))
        self.assertGreater(len(c.__data__), 0)
        self.assertTrue("name" in c.__data__)
        self.assertTrue("position" in c.__data__)
        self.assertFalse(hasattr(c.__data__, "invalidEntry"))
        self.assertTrue(isinstance(c.__data__, Sofa.Core.DataDict))

    def test_loggedMessagesBinding(self):
        node = create_scene("a_node")
        self.assertEqual(node.getLoggedMessagesAsString(), "")
        Sofa.msg_info(node, "This is a new message")
        self.assertTrue("This is a new message" in node.getLoggedMessagesAsString())
        self.assertEqual(node.countLoggedMessages(), 1)
        node.clearLoggedMessages()
        self.assertEqual(node.countLoggedMessages(), 0)

    def test_addNewData_with_type(self):
        node = create_scene("a_node")
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
        node = create_scene("a_node")
        self.assertRaises(ValueError, node.addObject, "MechanicalObject", name="children", position=[[0,0,0],[1,1,1],[2,2,2]])
        self.assertRaises(ValueError, node.addChild, "parents")
        self.assertRaises(ValueError, node.addData, name="links", type="int", value=42)

    def test_addNewDataFromParent_linkPaths(self):
        root = create_scene('root')
        c1 = root.addObject("MechanicalObject", name="c1")
        c1.addData("d", value="coucou", type="string")

        c2 = root.addObject("MechanicalObject", name="c2")
        c2.addData("data1", value="@/c1.d") # absolute path
        c2.addData("data2", value="@c1.d") # relative path (local)
        c2.addData("data3", value="@./c1.d") # relative path (local)
        self.assertEqual(c2.data1.value, c1.d.value)
        self.assertEqual(c2.data2.value, c1.d.value)
        self.assertEqual(c2.data3.value, c1.d.value)

        n1 = root.addChild("n1")
        n2 = root.addChild("n2")

        c3 = n1.addObject('MechanicalObject', name='c3')
        c3.addData("data1", value="@../c1.d") # relative path (down)
        self.assertEqual(c3.data1.value, c1.d.value)

        c4 = n2.addObject('MechanicalObject', name='c4')
        c4.addData('data1', value="@/n1/c3.data1") # absolute path (chained)
        c4.addData('data2', value="@../n1/c3.data1") # relative path (down, chained)
        self.assertEqual(c4.data1.value, c3.data1.value)
        self.assertEqual(c4.data2.value, c3.data1.value)

        c3.addData('data2', value="test", type="string")
        c1.addData('data4', value="@n1/c3.data2") # relative (up, chained)
        self.assertEqual(c1.data4.value, c3.data2.value)


    def test_addNewDataFromParent(self):
        root = create_scene('root')
        c1 = root.addObject("MechanicalObject", name="c1")
        c1.addData("d", value="coucou", type="string")
        c1.addData("d2", value="@c1.d")

        # Data exists
        self.assertTrue(hasattr(c1, "d"))
        # Data correctly took its parent value
        self.assertEqual("coucou", c1.d2.value)
        # parent relationship is kept
        self.assertEqual(c1.d2.getParent(), c1.d)

        c1.d = "test"
        # child properly updated from parent value
        self.assertEqual("test", c1.d2.value)


    def test_addNewDataFromParent_brokenLink(self):
        root = create_scene('root')
        c1 = root.addObject("MechanicalObject", name="c1")

        ValueError_ToTest = ["@aBroken/path.value", "@aBroken/path", "@/aBroken/path", "@", "@.", "@/", "@./", "@../"]
        TypeError_ToTest = ["", ".", "/", "./", "../"]
        # Error mgmt testing

        for val in ValueError_ToTest:
            self.assertRaises(ValueError, c1.addData, name="d", value=val)

        for val in TypeError_ToTest:
            self.assertRaises(TypeError, c1.addData, name="d", value=val)

    def test_getClassName(self):
        root = create_scene("root")
        self.assertEqual(root.getClassName(), "Node")

    def test_getTemplateName(self):
        root = create_scene("root")
        c = root.addObject("MechanicalObject", name="t")
        self.assertEqual(c.getTemplateName(),"Vec3d")

    def test_getLinkPath(self):
        root = create_scene("root")
        obj = root.addObject("MechanicalObject", name="obj")
        self.assertEqual(obj.getPathName(),"/obj")
        self.assertEqual(obj.getLinkPath(),"@/obj")

    def test_addExistingDataAsParentOfNewData(self):
        # TODO(@marques-bruno)
        # do a test like this:
        # obj1 = root.addObject('AComposant', aDataField="pouet")
        # obj2 = root.addObject('AnotherComposant')
        # obj2.addData(obj1.aDataField)
        # self.assertTrue(hasattr(obj2, "aDataField"))
        # self.assertEqual(obj2.an_objectName.getParent(), obj1.aDataField)
        # self.assertEqual(obj2.an_objectName.value, "pouet")

        # And another one like this:
        # aData = createAnOrphanData(name="MyData", value="pouet", type="str")
        # obj1 = root.addObject('AComposant', aDataField="pouet")
        # obj1.addData(aData)
        # self.assertTrue(hasattr(obj1, "aData"))
        # self.assertEqual(obj2.aData.getOwner(), obj1)
        # self.assertEqual(aData.getOwner(), obj1)
        # self.assertEqual(obj2.aData.value, "pouet")
        pass

