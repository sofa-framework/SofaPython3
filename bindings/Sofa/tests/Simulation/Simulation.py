# -*- coding: utf-8 -*
import Sofa
import unittest
import Sofa.Core
import Sofa.Types
import Sofa.Simulation
import tempfile
import os

class Test(unittest.TestCase):
    def test_load(self):
        """Create a scene and load it to validate the loading function"""
        scene="""
            <Node name="rootNode" dt="0.005" gravity="0 0 0">
                <Node name="child1">
                </Node>
                <Node name="child2">
                </Node>
            </Node>
        """
        tf = tempfile.NamedTemporaryFile(mode="w+t", suffix=".scn", delete=False)
        tf.write(scene)
        tf.flush()
        tf.close() # need to close the file otherwise SOFA cannot open it (on Windows)
        node = Sofa.Simulation.load(tf.name)
        self.assertNotEqual(node, None)
        self.assertEqual(node.name.value, "rootNode")
        os.remove(tf.name)

