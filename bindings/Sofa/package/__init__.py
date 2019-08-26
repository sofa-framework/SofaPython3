"""
Sofa binding
-----------------------

Example of use:
  .. code-block:: python

    import Sofa.Core
    import Sofa.Simulation
    import SofaRuntime
    SofaRuntime.importPlugin("SofaAllCommonComponents")

    n = Sofa.Core.Node("MyNode")
    n.addChild("Node2")
    n.addObject("MechanicalObject", name="dofs")

    Sofa.Simulation.init(n)
    Sofa.Simulation.print(n)

Submodules:
  .. autosummary::
    :toctree: _autosummary

    Sofa.Core
    Sofa.Simulation
    Sofa.Types
    Sofa.Helper
"""
