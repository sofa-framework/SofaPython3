"""
Sofa binding
-----------------------

Example of use:
  .. code-block:: python

     import Sofa

     n = Sofa.Core.Node("MyNode"")
     n.addChild("Node2")
     n.addObject("MechanicalObject", name="dofs")

     Sofa.Simulation.init(root)
     Sofa.Simulation.print(root)

Submodules:
  .. autosummary::
    :toctree: _autosummary

    Sofa.Core
    Sofa.Simulation
    Sofa.Types
    Sofa.Helper
"""
