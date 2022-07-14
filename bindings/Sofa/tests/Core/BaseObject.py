# coding: utf8

import unittest
import Sofa

def create_scene(rootName="root"):
    root = Sofa.Core.Node(rootName)
    root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
    return root

class Test(unittest.TestCase):
    def __init__(self, a):
        unittest.TestCase.__init__(self, a)

    def test_createObjectWithParam(self):
        root = create_scene("rootNode")
        c = root.addObject("MechanicalObject", name="t", position=[
                           [0, 0, 0], [1, 1, 1], [2, 2, 2]])
        self.assertTrue(c is not None)

    def test_createObjectWithInvalidParamName(self):
        # This one should raise an error because of 'v' should rise a type error.
        root = create_scene("rootNode")
        self.assertRaises(TypeError, root.addObject, "MechanicalObject", name="tt", v=[
                          [0, 0, 0], [1, 1, 1], [2, 2, 2]])

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

    def test_context(self):
        root = create_scene("rootNode")
        c = root.addObject("MechanicalObject", name="t", position=[
                           [0, 0, 0], [1, 1, 1], [2, 2, 2]])
        self.assertTrue(c.getContext() is not None)

    def test_slave_master(self):
        root = create_scene("rootNode")
        c_master = root.addObject("MechanicalObject", name="t_master", position=[
                                  [0, 0, 0], [1, 1, 1], [2, 2, 2]])
        c_slave = root.addObject("MechanicalObject", name="t_slave", position=[
                                 [0, 0, 0], [1, 1, 1], [2, 2, 2]])
        c_master.addSlave(c_slave)
        self.assertEqual(c_master, c_slave.getMaster())
        self.assertEqual(c_master.getSlaves()[0], c_slave)
        self.assertEqual(c_slave.getSlaves(), [])

    def test_getTarget(self):
        root = create_scene("rootNode")
        c = root.addObject("MechanicalObject", name="t")
        self.assertEqual(c.getTarget(), 'Sofa.Component.StateContainer')

    def test_getName(self):
        root = create_scene("rootNode")
        c = root.addObject("MechanicalObject", name="t")
        self.assertEqual(c.getName(), "t")

    def test_getCategories(self):
        root = create_scene("rootNode")
        c = root.addObject("MechanicalObject", name="t")
        self.assertEqual(c.getCategories(), ["MechanicalState"])

    def test_getPathName(self):
        root = create_scene("rootNode")
        c = root.addObject("MechanicalObject", name="t")
        self.assertEqual(c.getPathName(), "/t")

    def test_getLinkPath(self):
        root = create_scene("rootNode")
        c = root.addObject("MechanicalObject", name="t")
        self.assertEqual(c.getLinkPath(), "@/t")
        self.assertEqual(c.getAsACreateObjectParameter(), "@/t")

    def test_setSrc(self):
        root = create_scene("rootNode")
        c = root.addObject("Binding_BaseObject_MockComponent", name="t")
        loader = root.addObject(
            "Binding_BaseObject_MockComponent", name="loader")
        loader.test = "setSrc"
        c.setSrc("test", loader)
        self.assertEqual(c.test.value, loader.test.value)

    def test_mockComponent(self):
        # Test several binded functions using the Binding_BaseObject_MockComponent
        t = ["bwdInit", "cleanup", "computeBBox",
             "storeResetState", "reset", "init", "reinit"]
        root = create_scene("rootNode")
        c = root.addObject("Binding_BaseObject_MockComponent", name="t")
        for name in t:
            getattr(c, name)()
            self.assertEqual(c.test.value, name)
