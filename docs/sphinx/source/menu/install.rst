Download and Install
================================

The plugin can be compiled from the source. 

Install from source
-------------------

Requirement Install
^^^^^^^^^^^^^^^^^^^
- pybind11 (minimal 2.2.4)
- cmake (minimal 3.12)
- developement package for python3.7 (python3.7-dev)
- numpy

In-tree build
^^^^^^^^^^^^^^^^^^^
Add this directory path in `CMAKE_EXTERNAL_DIRECTORIES`.

NB: This plugin cannot be build through in-build process when the old SofaPython plugin is activated. To have both SofaPython3 and SofaPython you need to use out-of-tree build. 

Out-of-tree build
^^^^^^^^^^^^^^^^^^^

This plugin should compile with out-of-tree builds



