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

If you have already compiled and installed SOFA, you will have to do it again, and add some configuration to CMAKE:

	* Add this directory path in `SOFA_EXTERNAL_DIRECTORIES`.
	* Check the `PLUGIN_SOFAPYTHON3` box
	* You may need to add an entry called `SofaPython3_DIR`, of the value <SOFA_build>/cmake



You may also need to add to your variable `PYTHONPATH` the string: <SOFA_build>/lib/site-packages


NB: This plugin cannot be build through in-build process when the old SofaPython plugin is activated. To have both SofaPython3 and SofaPython you need to use out-of-tree build. 

Out-of-tree build
^^^^^^^^^^^^^^^^^^^

This plugin should compile with out-of-tree builds



