The modules
================================

Several modules are available. Each exposing different aspect of the Sofa API and components. 

These modules can be loaded either in an existing sofa application with the SofaPython3 plugins loaded or 
in python3 interpretor (CPython, ipython, jupyter).
      
  .. code-block:: python
  
        # encoding: utf-8
        #!/usr/bin/python3
        """ A minimalistic application using the Sofa module to create a scene 
            and the SofaRuntime module to execute it
        """
        import os
         
        if "SOFA_ROOT" not in os.environ:
                print("WARNING: missing SOFA_ROOT in you environment variable. ") 
                sys.exit(-1)

        import Sofa
        import SofaRuntime
                              
        root = Sofa.Node("root") 
        root.addChild("MechanicalObject", name="dofs")
        
        Sofa.Simulation.init(root)
        Sofa.Simulation.print(root)

        for i in range(0, 10):
                Sofa.Simulation.animate(root, 0.1)
        
Available modules: 
------------------
.. autosummary::
    :toctree: _autosummary

    Sofa
    SofaRuntime   
 

