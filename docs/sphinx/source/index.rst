General overview
================

The SofaPython3 project brings python bindings for the `SOFA simulation framework <https://www.sofa-framework.org>`_.
Thereby, it allows users to access most of the components and API found in SOFA directly into their python scripts.

This project is composed of two modules: *Sofa plugin* and several *python modules*.

1. The **SofaPython3 plugin** which embeds a python interpreter and a scene loader. This plugin allows one to load a python
script as the main simulation scene file. The scene loader will automatically detect the ".py", ".py3", ".py3scn" or
".pyscn" file extension and load the script into the embedded python interpreter.

2. The **SofaPython3 bindings** which make the link between the SOFA API and python. These are compiled (C++) python
modules compatible with the C-Python ABI and can therefore be directly imported into any python interpreter, given
that the python version is compatible. Hence, the python interpreter embedded inside the *SOFA plugin* will be able
to load these binding modules. In addition, one could load the binding modules directly inside their own python
interpreter without requiring the load of the plugin.

.. figure:: images/SP3_global_arch.png
    :alt: How SP3 fits in the SOFA usage pipeline
    :align: center

Content
-------

.. toctree::
   :glob:
   :maxdepth: 1

   menu/Installation
   menu/Compilation
   menu/SofaPlugin
   menu/SofaModule
   menu/pythonLibraries
   menu/CustomModule
   menu/Contributing

   
