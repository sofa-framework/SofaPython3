# coding: utf8

import Sofa
import Sofa.Helper
import unittest
import os
from pathlib import Path
from tempfile import gettempdir


class Test(unittest.TestCase):
    def __init__(self, methodName):
        unittest.TestCase.__init__(self, methodName)

        # First temporary directory
        self.temp_dir1 = os.path.join(gettempdir(), f'FileRepository_{hash(os.times())}_test_1')
        os.makedirs(self.temp_dir1, exist_ok=True)
        self.temp_file1 = open(os.path.join(self.temp_dir1, "a_simple_test_file_1.txt"), "w")

        # Second temporary directory
        self.temp_dir2 = os.path.join(gettempdir(), f'FileRepository_{hash(os.times())}_test_2')
        os.makedirs(self.temp_dir2, exist_ok=True)
        self.temp_file2 = open(os.path.join(self.temp_dir2, "a_simple_test_file_2.txt"), "w")

        self.repository = Sofa.Helper.System.FileRepository()

    def test_environment(self):
        os.environ['FILEREPOSITORYTEST'] = self.temp_dir1
        self.repository = Sofa.Helper.System.FileRepository("FILEREPOSITORYTEST")

        self.assertEqual([Path(p) for p in self.repository.getPaths()], [Path(self.temp_dir1)])
        self.repository.clear()
        self.assertEqual(self.repository.getPaths(), [])

    def test_add_remove_directories(self):
        self.repository.addFirstPath(self.temp_dir1)
        self.repository.addFirstPath(self.temp_dir2)
        self.assertEqual([Path(p) for p in self.repository.getPaths()], [Path(self.temp_dir2), Path(self.temp_dir1)])

        self.repository.removePath(str(Path(self.temp_dir2).as_posix()))
        self.assertEqual([Path(p) for p in self.repository.getPaths()], [Path(self.temp_dir1)])

        self.repository.addLastPath(self.temp_dir2)
        self.assertEqual([Path(p) for p in self.repository.getPaths()], [Path(self.temp_dir1), Path(self.temp_dir2)])

        self.repository.clear()

    def test_find_file(self):
        self.repository.addFirstPath(self.temp_dir1)
        self.assertTrue(self.repository.findFile("a_simple_test_file_1.txt"))
        self.repository.addFirstPath(self.temp_dir2)
        self.assertTrue(self.repository.findFile("a_simple_test_file_2.txt"))

        self.assertEqual(Path(self.repository.getFile("a_simple_test_file_1.txt")), Path(os.path.join(self.temp_dir1, "a_simple_test_file_1.txt")))
        self.assertEqual(Path(self.repository.getFile("a_simple_test_file_2.txt")), Path(os.path.join(self.temp_dir2, "a_simple_test_file_2.txt")))

        self.repository.clear()

