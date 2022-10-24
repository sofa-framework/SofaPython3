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
        self.iterations = 0

    def __del__(self):
            pass
    
    def init(self):
            self.inited += 1

    def onEvent(self, kwargs):
            pass

    def onBuildConstraintSystemEndEvent(self, kwargs):
            self.iterations+=1


class Test(unittest.TestCase):

    def test_events(self):
        """Test the BuildConstraintSystem event."""
        node = Sofa.Core.Node("root")
        node.addObject("DefaultAnimationLoop", name="loop")
        controller = node.addObject( MyController() )

        self.assertTrue( hasattr(controller, "iterations") )

        Sofa.Simulation.init(node)
        for i in range(10):
            Sofa.Simulation.animate(node, 0.01)

        self.assertTrue( hasattr(controller, "iterations") )
        self.assertEqual( controller.iterations, 10 )


    @staticmethod
    def simulate_beam(linear_solver_template):
        root = Sofa.Core.Node("rootNode")

        root.addObject('DefaultAnimationLoop')

        root.addObject('EulerImplicitSolver', rayleighStiffness="0.1", rayleighMass="0.1")
        root.addObject('SparseLDLSolver', applyPermutation="false", template=linear_solver_template)

        return root

