# coding: utf8
import Sofa
import numpy
import unittest


class MyPrefab(Sofa.Prefab):
    prefabParameters = [
        {"name" : "myParameter", "type" : "int", "help" : "my message to help the user", "default" : 0}
    ]

    prefabData = [
        {"name" : "myData", "type" : "Vec3d", "help" : "my message to help the user", "default" : [0.0,0.0,0.0]}
    ]

    initCount = 0

    def __init__(self, *args, **kwargs):
        Sofa.Prefab.__init__(self, *args, **kwargs)

    def init(self):
        for i in range(self.myParameter.value):
            self.addChild("Child"+str(i))
        self.addObject("MechanicalObject", name="dofs", position=self.myData.getLinkPath(), showObject=True, showObjectScale=2)
        self.initCount += 1

class Test(unittest.TestCase):
    def test_prefab_init(self):
        root = Sofa.Core.Node("root")
        root.addChild(MyPrefab(name="prefab", myParameter=3, myData=[1.0,2.0,3.0]))

        self.assertEqual(len(root.prefab.children), 3)
        numpy.testing.assert_array_equal(root.prefab.myData.value, [1.0,2.0,3.0])
        numpy.testing.assert_array_equal(root.prefab.dofs.position.value, [[1.0,2.0,3.0]])

    def test_prefab_reinit(self):
        root = Sofa.Core.Node("root")
        root.addChild(MyPrefab(name="prefab", myParameter=3, myData=[1.0,2.0,3.0]))

        self.assertEqual(len(root.prefab.children), 3)
        self.assertEqual(root.prefab.initCount, 1)

        root.prefab.myParameter = 5
        self.assertEqual(len(root.prefab.children), 5)
        self.assertEqual(root.prefab.initCount, 2)

        root.prefab.myParameter = 6
        self.assertEqual(len(root.prefab.children), 6)
        self.assertEqual(root.prefab.initCount, 3)
