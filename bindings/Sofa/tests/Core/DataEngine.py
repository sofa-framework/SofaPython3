# This Python file uses the following encoding: utf-8
# coding: utf8

import unittest
import gc
import Sofa.Core
import Sofa.Simulation

class MyDataEngine(Sofa.Core.DataEngine):
    """This is a custom data engine
    when init is called from Sofa this should call the python init function
    when update is called from Sofa this should call this engine's doUpdate function
    """

    def __init__(self, *args, **kwargs):
        ## These are needed (and the normal way to override from a python class)
        Sofa.Core.DataEngine.__init__(self, *args, **kwargs)
        self.addData(name="d_in", value=kwargs.get("d_in"), type="int", help="an input from another component")
        self.addData(name="d_out", value=kwargs.get("d_out"), default=self.d_in.value, type="int", help="an output to another component")
        self.counter = 0

    def init(self):
        self.addInput(self.d_in)
        self.addOutput(self.d_out)

    def update(self):
        self.d_out = self.d_in.value + 1
        self.counter += 1

class Test(unittest.TestCase):
     def test_constructor(self):
         c = Sofa.Core.DataEngine()
         c.init()

     def test_constructorOverriden(self):
         root = Sofa.Core.Node("rootNode")
         root.addObject(MyDataEngine(name="dataEngine", d_in=41))
         root.dataEngine.init()
         root.dataEngine.update()
         self.assertEqual(root.dataEngine.d_out.value, 42)

     def test_DataEngine(self):
         class MyController(Sofa.Core.Controller):
             def __init__(self, *a, **kw):
                 Sofa.Core.Controller.__init__(self, *a, **kw)
                 self.addData(name="v", value=kw.get("v"), type="int")


         root = Sofa.Core.Node("rootNode")
         root.addObject(MyController(name="inputController", v=40))
         root.addObject(MyDataEngine(name="dataEngine", d_in=root.inputController.v))
         root.addObject(MyController(name="outputController", v=root.dataEngine.d_out))

         for obj in root.objects:
             obj.init()

         # engine triggers, because input initialized to dirty by default
         self.assertEqual(root.outputController.v.value, 41)
         self.assertEqual(root.dataEngine.counter, 1)

         # engine will not trigger, as input did not change
         self.assertEqual(root.outputController.v.value, 41)
         self.assertEqual(root.dataEngine.counter, 1)

         root.inputController.v = 41 # modify input

         # counter stays the same as value was not yet accessed
         self.assertEqual(root.dataEngine.counter, 1)

         # engine must have triggered, value should be 42
         self.assertEqual(root.outputController.v.value, 42)
         self.assertEqual(root.dataEngine.counter, 2)
         return

