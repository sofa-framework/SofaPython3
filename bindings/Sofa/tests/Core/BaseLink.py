# coding: utf8


import unittest
import Sofa

def create_scene(rootName="root"):
    root = Sofa.Core.Node(rootName)
    root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
    root.addObject("RequiredPlugin", name="Sofa.Component.SolidMechanics.Spring")
    return root

class Test(unittest.TestCase):
    def __init__(self, a):
        unittest.TestCase.__init__(self, a)

    def test_Name(self):
        root = create_scene("root")
        root.addObject("MechanicalObject", name="t")
        link = root.findLink("mechanicalState")
        self.assertEqual(link.getName(),"mechanicalState")
        link.setName("newName")
        self.assertEqual(link.getName(),"newName")

    def test_Help(self):
        root = create_scene("root")
        root.addObject("MechanicalObject", name="t")
        link = root.findLink("mechanicalState")
        self.assertEqual(link.getHelp(), 'The MechanicalState attached to this node (storing all state vectors)')
        link.setHelp("new help")
        self.assertEqual(link.getHelp(), 'new help')

    def test_isMultiLink(self):
        root = create_scene("root")
        root.addObject("MechanicalObject", name="t")
        link = root.findLink("mechanicalState")
        self.assertFalse(link.isMultiLink())
        root.addChild("aChild")
        link = root.findLink("child")
        self.assertTrue(link.isMultiLink())

    def test_Persistent(self):
        root = create_scene("root")
        root.addObject("MechanicalObject", name="t")
        link = root.findLink("mechanicalState")
        self.assertFalse(link.isPersistent())
        self.assertTrue(link.isReadOnly())
        link.setPersistent(True)
        self.assertTrue(link.isPersistent())
        self.assertFalse(link.isReadOnly())

    def test_getSize(self):
        root = create_scene("root")
        link = root.findLink("child")
        self.assertEqual(link.getSize(),0)
        root.addChild("child1")
        link = root.findLink("child")
        self.assertEqual(link.getSize(),1)
        root.addChild("child2")
        link = root.findLink("child")
        self.assertEqual(link.getSize(),2)

    def test_getValue(self):
        root = create_scene("root")
        root.addChild("child1")
        link = root.findLink("child")
        self.assertEqual(link.getValueString(), "@/child1")
        self.assertEqual(link.getValueTypeString(), "Node")

    def test_getLinkedPath(self):
        root = create_scene("root")
        c1 = root.addChild("child1")
        link = root.findLink("child")
        self.assertEqual(link.getLinkedPath(0), "@/child1")

    def test_getOwnerBase(self):
        root = create_scene("theRoot")
        link = root.findLink("child")
        self.assertEqual(link.getOwnerBase().name.value,"theRoot")

    def test_getLinkedBase(self):
        root = create_scene("root")
        root.addChild("aChild")
        link = root.findLink("child")
        self.assertEqual(link.getLinkedBase(0).name.value,"aChild")

    def test_getLinked_and_Owner(self):
        root = create_scene("root")
        c1 = root.addObject("MechanicalObject", name="t1")
        c2 = root.addObject("MechanicalObject", name="t2")
        mm  = root.addObject("BarycentricMapping", input="@/t1", output="@/t2")
        link_input = mm.findLink("input")
        self.assertEqual(link_input.getLinkedBase(0).getName(),"t1")
        link_output = mm.findLink("output")
        self.assertEqual(link_output.getLinkedBase(0).getName(),"t2")
        self.assertEqual(link_input.getOwnerBase().getName(), mm.getName())
        self.assertEqual(link_output.getOwnerBase().getName(), mm.getName())

    def test_baselink_attributes_forwarding(self):
        root = create_scene("root")
        c1 = root.addObject("MechanicalObject", name="t1", position=[[-1,-2,-3],[-4,-5,-6]])
        f1 = root.addObject("RestShapeSpringsForceField", name="forcefield", mstate="@t1")
        self.assertEqual(len(f1.mstate.position), 2)
        f1.mstate.position = [[1.0,2.0,3.0]]
        self.assertEqual(len(f1.mstate.position), 1)

    @unittest.skip # Segmentation fault on MacOS
    def test_read(self):
        root = create_scene("root")
        c1 = root.addObject("MechanicalObject", name="t1")
        link = root.findLink("object")
        self.assertEqual(link.getValueString(),"@//t1", F"Link value should be '@//t1', but it is {link.getValueString()}")
        c2 = root.addObject("MechanicalObject", name="t2")
        self.assertTrue(link.read("@/t2"), "Failed to read '@/t2'")
        self.assertEqual(link.getValueString(),"@//t2", F"Link value should be '@//t2', but it is {link.getValueString()}")
