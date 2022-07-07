# -*- coding: utf-8 -*
import Sofa
import unittest
import Sofa.Types
import Sofa.Simulation
import SofaRuntime

class MyController(Sofa.Core.Controller):
        """This is my custom controller
           when init is called from Sofa this should call the python init function
           This controller is used to test the method sendEvent
        """
        def __init__(self, *args, **kwargs):
            ## These are needed (and the normal way to override from a python class)
            Sofa.Core.Controller.__init__(self, *args, **kwargs)
            self.event_name = ""
            self.userData = ""
            self.sender = ""

        def onPythonScriptEvent(self, kwargs):
            self.event_name = kwargs.get("event_name")
            self.userData = kwargs.get("userData")
            self.sender = kwargs.get("sender")

class Test(unittest.TestCase):
        def test_SimulationConstructor(self):
            root = Sofa.Core.Node("rootNode")
            self.assertEqual(root.name.value, "rootNode")

        def test_Constructor(self):
                root = Sofa.Core.Node("rootNode")
                self.assertEqual(root.name.value, "rootNode")

        def test_GetAttr(self):
                root = Sofa.Core.Node("rootNode")
                root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
                c = root.addChild("child1")
                self.assertTrue(c is not None)
                self.assertTrue(root.child1 is not None)
                
                o = c.addObject("MechanicalObject", name="mechanical")
                self.assertTrue(o is not None)
                self.assertTrue(root.child1.mechanical is not None)

        def test_init(self):
                root = Sofa.Core.Node("rootNode")
                root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
                c = root.addChild("child1")
                c = c.addObject("MechanicalObject", name="MO", position=[0.0,1.0,2.0]*100)
                root.init()
                print("TYPE: "+str(len(c.position.value)))
                self.assertEqual(len(c.position.value), 100)

        def test_createObjectInvalid(self):
                root = Sofa.Core.Node("rootNode")
                c = root.addChild("child1")
                def d():
                    c.addObject([1.0,0.0,0.0])
                self.assertRaises(TypeError, d)

                def d1():
                    c.addObject("NOEXISTINGCOMPONENT")

                self.assertRaises(ValueError, d1)
                root.init()
                
        def test_addChild(self):                
                root = Sofa.Core.Node("rootNode")
                root.addChild(Sofa.Core.Node("child1"))
                self.assertTrue(hasattr(root,"child1"))

        def test_removeChild(self):                
                root = Sofa.Core.Node("rootNode")
                c = root.addChild(Sofa.Core.Node("child1"))
                c2 = root.addChild(Sofa.Core.Node("child2"))
                self.assertEqual(len(root.children), 2)
                self.assertTrue(hasattr(root,"child1"))
                self.assertTrue(hasattr(root,"child2"))
                root.removeChild(c)
                self.assertEqual(len(root.children), 1)

                self.assertFalse(hasattr(root,"child1"))
                root.removeChild("child2")
                self.assertFalse(hasattr(root,"child2"))
                self.assertEqual(len(root.children), 0)

        def test_createObjectWithParam(self):
                root = Sofa.Core.Node("rootNode")
                root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
                root.addObject("MechanicalObject", name="mechanical", position=[[0,0,0],[1,1,1],[2,2,2]])
                        
        def test_children_property(self):                
                root = Sofa.Core.Node("rootNode")
                c1 = root.addChild(Sofa.Core.Node("child1"))
                self.assertEqual(len(root.children), 1)
                c2 = root.addChild(Sofa.Core.Node("child2"))
                self.assertEqual(len(root.children), 2) 

                self.assertEqual(root.children.at(0).name, c1.name)
                self.assertEqual(root.children.at(1).name, c2.name)
                root.children.remove_at(0)
                self.assertEqual(len(root.children), 1)
                self.assertEqual(root.children.at(0).name, c2.name)

                self.assertTrue(isinstance(root.children[0], Sofa.Core.Node))

        def test_parents_property(self):                
                root = Sofa.Core.Node("rootNode")
                c1 = root.addChild(Sofa.Core.Node("child1"))
                c2 = root.addChild(Sofa.Core.Node("child2"))
                d = c1.addChild(Sofa.Core.Node("subchild"))
                d = c2.addChild(Sofa.Core.Node("subchild"))
                self.assertEqual(len(d.parents), 1)
                c1.addChild(d)
                self.assertEqual(len(d.parents), 2)

        def test_objects_property(self):
                root = Sofa.Core.Node("rootNode")
                root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
                child = root.addChild(Sofa.Core.Node("child1"))
                child.addObject("MechanicalObject", name="name1")
                child.addObject("MechanicalObject", name="name2")
                self.assertEqual(len(child.objects), 2)
                child.addObject("MechanicalObject", name="name2")
                self.assertEqual(len(child.objects), 3)

        def test_objects_property_contains_method(self):
            root = Sofa.Core.Node("rootNode")
            root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
            child = root.addChild(Sofa.Core.Node("child1"))
            child.addObject("MechanicalObject", name="name1")
            child.addObject("MechanicalObject", name="name2")
            self.assertTrue("child1" in root.children)
            self.assertFalse("child2" in root.children)
            self.assertTrue("name1" in child.objects)
            self.assertTrue("name2" in child.objects)
            self.assertFalse("name3" in child.objects)

        def test_objects_property_remove_at_method(self):
            root = Sofa.Core.Node("rootNode")
            root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
            child = root.addChild(Sofa.Core.Node("child1"))
            child.addObject("MechanicalObject", name="name1")
            child.addObject("MechanicalObject", name="name2")
            self.assertTrue("name1" in child.objects)
            self.assertTrue("name2" in child.objects)
            child.objects.remove_at(1)
            self.assertTrue("name1" in child.objects)
            self.assertFalse("name2" in child.objects)

        def test_data_property(self):
                root = Sofa.Core.Node("rootNode")
                self.assertTrue(hasattr(root, "__data__"))
                self.assertGreater(len(root.__data__), 0)
                self.assertTrue("name" in root.__data__)
                self.assertFalse(hasattr(root.__data__, "invalidEntry"))
                self.assertTrue(isinstance(root.__data__, Sofa.Core.DataDict))

        def test_getItem(self):
            root = Sofa.Core.Node("root")
            root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
            node1 = root.addChild('node1')
            object1 = root.addObject("MechanicalObject", name="object1")
            node2 = node1.addChild('node2')
            object2 = node2.addObject("MechanicalObject", name="object2")

            # All those are allowed syntaxes:
            self.assertEqual(root[".name"], root.name)
            self.assertEqual(root["name"], root.name)

            self.assertEqual(object1["name"], object1.name)
            self.assertEqual(object1[".name"], object1.name)
            self.assertEqual(object1["."].name, object1.name)
            self.assertEqual(object1[""].name, object1.name)

            self.assertEqual(root["node1"].name, node1.name)
            self.assertEqual(root["object1"], object1)
            self.assertEqual(root["node1.node2"].name, node2.name)
            self.assertEqual(root["name"], root.name)
            self.assertEqual(root["node1.node2.object2.name"], object2.name)

            self.assertEqual(root["node1.node2.object2.name"], root.node1.node2.object2.name)

        def test_getRoot(self):
            root = Sofa.Core.Node("root")
            node = root.addChild("node")
            self.assertEqual(node.getRoot(), root)

        def test_getRootPath(self):
            root = Sofa.Core.Node("root")
            node = root.addChild("node")
            self.assertEqual(node.getRootPath(),"../")

        def test_getLink(self):
            root = Sofa.Core.Node("root")
            node = root.addChild("node")
            node2 = node.addChild("node2")
            self.assertEqual(node.getLinkPath(),"@/node")
            self.assertEqual(node2.getLinkPath(), "@/node/node2")

        def test_hasObjectWithFastPath(self):
            root = Sofa.Core.Node("root")
            root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
            self.assertTrue(root.hasObject("Sofa.Component.StateContainer"))
            self.assertFalse(root.hasObject("NonExistingObjectName"))

        def test_hasObjectWithDefaultPythonFunction(self):
            root = Sofa.Core.Node("root")
            root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")

            self.assertTrue(hasattr(root, "Sofa.Component.StateContainer"))
            self.assertFalse(hasattr(root, "NonExistingObjectName"))

        def test_removeObject(self):
            root = Sofa.Core.Node("root")
            root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
            child = root.addChild(Sofa.Core.Node("child1"))

            obj1 = child.addObject("MechanicalObject", name="obj1")
            obj2 = child.addObject("MechanicalObject", name="obj2")
            self.assertEqual(len(child.objects), 2)
            self.assertTrue(hasattr(child,"obj1"))
            self.assertTrue(hasattr(child,"obj2"))
            child.removeObject(obj1)
            self.assertEqual(len(child.objects), 1)

            self.assertFalse(hasattr(child,"obj1"))
            child.removeObject(obj2)
            self.assertFalse(hasattr(child,"obj2"))
            self.assertEqual(len(child.objects), 0)

        def test_moveChild(self):
            root1 = Sofa.Core.Node("root1")
            root2 = Sofa.Core.Node("root2")
            child = Sofa.Core.Node("aChild")
            root1.addChild(child)
            self.assertEqual(len(root1.children), 1)
            self.assertTrue(hasattr(root1,"aChild"))
            root2.moveChild(child,root1)
            self.assertEqual(len(root2.children), 1)
            self.assertTrue(hasattr(root2,"aChild"))
            self.assertEqual(len(root1.children), 0)
            self.assertFalse(hasattr(root1,"aChild"))

        def test_isInitialized(self):
            root = Sofa.Core.Node("root")
            root.init()
            self.assertTrue(root.isInitialized())

        def test_getAsACreateObjectParameter(self):
            root = Sofa.Core.Node("root")
            node = root.addChild("node")
            node2 = node.addChild("node2")
            self.assertEqual(node.getAsACreateObjectParameter(),"@/node")
            self.assertEqual(node2.getAsACreateObjectParameter(), "@/node/node2")

        def test_detachFromGraph(self):
            root = Sofa.Core.Node("root")
            node = root.addChild("node")
            self.assertEqual(len(root.children),1)
            node.detachFromGraph()
            self.assertEqual(len(root.children),0)

        def test_getMass(self):
            root = Sofa.Core.Node("root")
            root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
            root.addObject("MechanicalObject")
            m = root.addObject("UniformMass", name="mass", vertexMass=0.1)
            self.assertEqual(m,root.getMass())

        def test_getForceField(self):
            root = Sofa.Core.Node("root")
            root.addObject("RequiredPlugin", name="Sofa.Component.MechanicalLoad")
            root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
            root.addObject("MechanicalObject")
            ff = root.addObject('ConstantForceField', template="Vec3d", name="cff2")
            self.assertEqual(ff, root.getForceField(0))

        def test_getMechanicalState(self):
            root = Sofa.Core.Node("root")
            root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
            c = root.addObject("MechanicalObject")
            self.assertEqual(c, root.getMechanicalState())

        def test_getMechanicalMapping(self):
            root = Sofa.Core.Node("root")
            root.addObject("RequiredPlugin", name="Sofa.Component.StateContainer")
            root.addObject("MechanicalObject", name="t1")
            root.addObject("MechanicalObject", name="t2")
            mm  = root.addObject("BarycentricMapping", input="@/t1", output="@/t2")
            self.assertEqual(mm, root.getMechanicalMapping())

        def test_sendEvent(self):
            root = Sofa.Core.Node("rootNode")
            node = root.addChild("node")
            c = node.addObject(MyController(name="controller"))
            root.sendEvent(root.name,"This is a test")
            self.assertEqual(c.event_name, "This is a test")
            self.assertEqual(c.userData, root.name)
            self.assertEqual(c.sender, root)
