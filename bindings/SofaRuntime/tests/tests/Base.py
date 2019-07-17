# coding: utf8

import Sofa
import unittest

class Test(unittest.TestCase):
    def test_moduleImport(self):
        import SofaRuntime

        SofaRuntime.Input.isKeyPressed(0)
        SofaRuntime.Input.isMouseButtonPressed(0)
        SofaRuntime.Input.isMouseButtonPressed(1)
        SofaRuntime.Input.isMouseButtonPressed(2)

        SofaRuntime.Input.mousePosition
