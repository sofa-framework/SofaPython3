# coding: utf8

import unittest
import Sofa
import Sofa.Core
import Sofa.Simulation
import SofaRuntime


class MyController(Sofa.Core.Controller):
    """This is my custom controller
        when init is called from Sofa this should call the python init function
    """        

    def __init__(self, *args, **kwargs):
        ## These are needed (and the normal way to override from a python class)
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.inited = 0
        self.build_iterations = 0
        self.solve_iterations = 0

    def __del__(self):
            pass
    
    def init(self):
            self.inited += 1

    def onEvent(self, kwargs):
            pass

    def onBuildConstraintSystemEndEvent(self, kwargs):
            self.build_iterations+=1

    def onSolveConstraintSystemEndEvent(self, kwargs):
            self.solve_iterations+=1

class Test(unittest.TestCase):

    def test_events(self):
        """Test the BuildConstraintSystem and SolveConstraintSystem events."""
        node = Sofa.Core.Node("root")
        node.addObject("DefaultAnimationLoop", name="loop")
        controller = node.addObject( MyController() )

        self.assertTrue( hasattr(controller, "iterations") )

        Sofa.Simulation.init(node)
        for i in range(10):
            Sofa.Simulation.animate(node, 0.01)

        self.assertTrue( hasattr(controller, "build_iterations") )
        self.assertEqual( controller.build_iterations, 10 )

        self.assertTrue( hasattr(controller, "solve_iterations") )
        self.assertEqual( controller.solve_iterations, 10 )


