import unittest
import Sofa

class Test(unittest.TestCase):

    def test_dump_json(self):

        json = Sofa.Core.ObjectFactory.dump_json()
        self.assertIn("MechanicalObject", json, "MechanicalObject is not found in the json dump of the ObjectFactory")
