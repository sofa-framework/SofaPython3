Welcome to SofaPython3's documentation!
================================

The plugin is composed of a Sofa plugin to embed a python interpreter into a Sofa based simulation and several python modules that exposes 
the different c++ components used in Sofa. 

Using SofaPyton3 in runSofa:

        First you need to load the SofaPython3 plugin in your runSofa environement. This can be done using the GUI or with the autoloading feature. 


Using SofaPython3 from python3 (ipython, jupyter): 
    
  .. code-block:: python
  
        # encoding: utf-8
        #!/usr/bin/python3
        import sys
        import os

         
        if "SOFA_ROOT" not in os.environ:
                print("WARNING: missing SOFA_ROOT in you environment variable. ") 
                sys.exit(-1)

        sys.path.append(os.path.abspath("./bindings/Sofa/package"))
        sys.path.append(os.path.abspath("./bindings/SofaRuntime/package"))
        sys.path.append(os.path.abspath("./bindings/SofaTypes/package"))

        import Sofa
        import SofaRuntime

        ## Register all the common component in the factory. 
        SofaRuntime.importPlugin("SofaAllCommonComponents")

        class MyController(Sofa.Controller):
               def __init__(self, *args, **kwargs):
                       Sofa.Controller.__init__(self,*args, **kwargs)
                       print("INITED")
                
               def onEvent(self, event):
                       print("Event: "+event)
                              
        root = Sofa.Node("myroot") 
        root.addChild("child1")
        root.addObject(MyController())

        Sofa.Simulation.init(root)
        Sofa.Simulation.print(root)
        for i in range(0, 10):
                print("step: "+str(i))
                Sofa.Simulation.animate(root, 0.1)
        

Python modules: 

.. autosummary::
    :toctree: _autosummary

    Sofa
    SofaRuntime

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`


