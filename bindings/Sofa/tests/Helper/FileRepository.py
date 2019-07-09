# coding: utf8

import Sofa
import Sofa.Helper
import unittest
import tempfile
import os


class Test(unittest.TestCase):
    def __init__(self, methodName):
        unittest.TestCase.__init__(self, methodName)
        self.temp_dir1 = tempfile.TemporaryDirectory(suffix="Test", prefix="FileRepository")
        self.temp_file1 = open(os.path.join(self.temp_dir1.name, "a_simple_test_file_1.txt"), "w")

        self.temp_dir2 = tempfile.TemporaryDirectory(suffix="Test", prefix="FileRepository")
        self.temp_file2 = open(os.path.join(self.temp_dir2.name, "a_simple_test_file_2.txt"), "w")

        os.environ['FILEREPOSITORYTEST'] = self.temp_dir1.name
        self.repository = Sofa.Helper.System.FileRepository("FILEREPOSITORYTEST")
        self.assertEqual(self.repository.getPaths(), [self.temp_dir1.name])
        self.repository.clear()
        self.assertEqual(self.repository.getPaths(), [])

    def test_add_remove_directories(self):
        self.repository.addFirstPath(self.temp_dir1.name)
        self.repository.addFirstPath(self.temp_dir2.name)
        self.assertEqual(self.repository.getPaths(), [self.temp_dir2.name, self.temp_dir1.name])

        self.repository.removePath(self.temp_dir2.name)
        self.assertEqual(self.repository.getPaths(), [self.temp_dir1.name])

        self.repository.addLastPath(self.temp_dir2.name)
        self.assertEqual(self.repository.getPaths(), [self.temp_dir1.name, self.temp_dir2.name])

        self.repository.clear()

    def test_find_file(self):
        self.repository.addFirstPath(self.temp_dir1.name)
        self.assertTrue(self.repository.findFile("a_simple_test_file_1.txt"))
        self.repository.addFirstPath(self.temp_dir2.name)
        self.assertTrue(self.repository.findFile("a_simple_test_file_2.txt"))

        self.assertEqual(self.repository.getFile("a_simple_test_file_1.txt"), os.path.join(self.temp_dir1.name, "a_simple_test_file_1.txt"))
        self.assertEqual(self.repository.getFile("a_simple_test_file_21.txt"), os.path.join(self.temp_dir2.name, "a_simple_test_file_2.txt"))

        self.repository.clear()


def runTests():
    suite = unittest.TestLoader().loadTestsFromTestCase(Test)
    return unittest.TextTestRunner(verbosity=2).run(suite).wasSuccessful()


def createScene(rootNode):
    runTests()
