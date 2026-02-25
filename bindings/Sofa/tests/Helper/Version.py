import Sofa
import Sofa.Helper
import unittest

class GetVersionTest(unittest.TestCase):
    """
    Contains unit tests for verifying the existence and format of the `GetVersion` method
    in the `Sofa.Helper.Utils` module.

    Includes tests to assert that the version is correctly formatted and meets the expected
    structure.
    """
    def test_exist(self):
        self.assertTrue(hasattr(Sofa.Helper.Utils, "GetVersion"))

    def test_version_format(self):
        version = Sofa.Helper.Utils.GetVersion()
        print(f"SOFA Version: {version}")
        self.assertTrue(version.startswith('v'))
        version = version.replace('v', '')
        self.assertTrue(version.count('.') == 1)
        major, minor = version.split('.')
        self.assertTrue(major.isdigit())
        self.assertTrue(minor.isdigit())
        self.assertEqual(len(major), 2)
        self.assertEqual(len(minor), 2)
