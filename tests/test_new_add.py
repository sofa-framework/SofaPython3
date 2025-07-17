import unittest 
import Sofa
import SofaRuntime
import Sofa.Core
import stlib

class ObjectDeclaration(object):
    ...

Sofa.Core.ObjectDeclaration = ObjectDeclaration

class MechanicalObject(ObjectDeclaration):
    pass

class TestNewAdd(unittest.TestCase):
    def test_add_node_with_node_type(self):
        root = Sofa.Core.Node("root")
        root.add(Sofa.Core.Node, name="aNodeA")
        self.assertEqual(len(root.children), 1)
        self.assertEqual(root.children[0].name.value, "aNodeA")

    def test_add_node_with_node_instance(self):
        root = Sofa.Core.Node("root")
        root.add(Sofa.Core.Node("aNodeB"))
        self.assertEqual(len(root.children), 1)
        self.assertEqual(root.children[0].name.value, "aNodeB")

    def test_add_object_with_string_type(self):
        root = Sofa.Core.Node("root")
        root.add("MechanicalObject", name="anObject1", position=[[1,2,3]])
        self.assertEqual(len(root.objects), 1)
        self.assertEqual(root.objects[0].name.value, "anObject1")
        self.assertEqual(root.objects[0].position.value.shape, (1,3))

    def test_add_object_with_object_type(self):
        root = Sofa.Core.Node("root")
        root.add(MechanicalObject, name="anObject2", position=[[1,2,3]])
        self.assertEqual(len(root.objects), 1)
        self.assertEqual(root.objects[0].name.value, "anObject2")
        self.assertEqual(root.objects[0].position.value.shape, (1,3))

    def test_automatic_name_generation(self):
        root = Sofa.Core.Node("root")
        root.add(MechanicalObject, position=[[1,2,3]])
        root.add(MechanicalObject, position=[[1,2,3]])
        root.add(MechanicalObject, position=[[1,2,3]])
        self.assertEqual(root.objects[0].name.value, "MechanicalObject")
        self.assertEqual(root.objects[1].name.value, "MechanicalObject1")
        self.assertEqual(root.objects[2].name.value, "MechanicalObject2")

        root.add(Sofa.Core.Node, name="TestNode")
        root.add(Sofa.Core.Node, name="TestNode")
        self.assertEqual(root.children[0].name.value, "TestNode")
        self.assertEqual(root.children[1].name.value, "TestNode1")

        root.add(Sofa.Core.Node)
        root.add(Sofa.Core.Node)
        self.assertEqual(root.children[2].name.value, "Node")
        self.assertEqual(root.children[3].name.value, "Node1")

    def test_add_node_with_kwargs(self):
        root = Sofa.Core.Node("root")
        root.add(Sofa.Core.Node, name="aNodeC", gravity=[1,2,3])
        self.assertEqual(root.children[0].gravity.value, [1,2,3])

    def test_add_instanciated_prefab(self):
        root = Sofa.Core.Node("root")
        from stlib.entities import Entity, EntityParameters

        bunnyParameters = EntityParameters()
        bunny = root.add(Entity(bunnyParameters))

    def test_add_prefab_with_parameter_object(self):
        root = Sofa.Core.Node("root")
        from stlib.entities import Entity, EntityParameters

        bunnyParameters = EntityParameters()
        bunny = root.add(Entity, bunnyParameters)


if __name__ == '__main__':

    SofaRuntime.importPlugin("Sofa.Component.StateContainer")
    unittest.main()
