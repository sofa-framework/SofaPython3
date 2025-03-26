import unittest
import Sofa
import SofaRuntime

class Test(unittest.TestCase):

    def test_dump_json(self):
        SofaRuntime.importPlugin("Sofa.Component.StateContainer")
        json = Sofa.Core.ObjectFactory.dump_json()
        self.assertIn("MechanicalObject", json, "MechanicalObject is not found in the json dump of the ObjectFactory")
