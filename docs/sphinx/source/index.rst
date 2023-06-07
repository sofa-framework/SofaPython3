The SofaPython3 project brings python bindings for the `SOFA simulation framework <https://www.sofa-framework.org>`_.
Thereby, it allows users to use most of the components and features found in SOFA directly into their python scripts.

This project is composed of two modules: 1. a *SOFA plugin* in the *SofaPython3/Plugin/* repository and 2. several *python modules* in the *SofaPython3/bindings/* repository.

1. The **SofaPython3 plugin** embeds a python interpreter and a SOFA scene loader. This plugin allows one to load a python
script as the main simulation scene file. The scene loader will automatically detect the ".py", ".py3", ".py3scn" or
".pyscn" file extension and load the script into the embedded python interpreter. Whereas traditional XML-based scene
file would discribe the SOFA scene structure, the python scene script will instead programmatically construct the SOFA
scene.

2. The **SofaPython3 bindings** makes the link between the SOFA API and python. These are compiled (C++) python
modules compatible with the C-Python ABI and therefore directly accessible from any python interpreter, given
that the python version is compatible. Hence, the python interpreter embedded inside the *SofaPython3 plugin* will be able
to load these binding modules. In addition, one could load the binding modules directly inside their own python
interpreter without requiring the load of the plugin.

.. figure:: images/SP3_global_arch.png
    :alt: How SP3 fits in the SOFA usage pipeline
    :align: center
    :width: 400

.. toctree::
    
    modules/index

