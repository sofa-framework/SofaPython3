Compilation
===========

The compilation of the SofaPython3 can be done automatically when `building SOFA <https://www.sofa-framework.org/community/doc/getting-started/build/linux/>`__,
or it can be done independently using an already compiled/installed SOFA. The former is called an *in-tree build*, which is,
the compilation of SofaPython3 is included inside the build tree of SOFA. The latter is called an *out-of-tree build*, hence
the compilation of SofaPython3 is done in its own build tree, usually placed somewhere else than SOFA. This page will
cover both methods.

Prerequisites
-------------
The compilation of SofaPython3 requires the installation of some dependencies. The following table lists all of those
prerequisites.

.. content-tabs::

    .. tab-container:: tab1
        :title: Ubuntu

        .. list-table::
            :widths: 15 10 30 45
            :header-rows: 1

            * - Package
              - Type
              - Installation
              - Description
            * - Python 3 libs
              - **Required**
              - ``sudo apt install python3-dev``
              - Used to link against the C-Python API
            * - `pybind11 <https://pybind11.readthedocs.io/en/stable/>`__
              - **Required**
              - ``sudo apt install pybind11-dev``
              - Heavily used to automatically create the binding code between SOFA and python
            * - `SOFA Framework <https://www.sofa-framework.org/community/doc/>`__
              - **Required**
              - See below for more information
              -


    .. tab-container:: tab2
        :title: MacOS

        .. list-table::
            :widths: 15 10 30 45
            :header-rows: 1

            * - Package
              - Type
              - Installation
              - Description
            * - Python 3 libs
              - **Required**
              - ``brew install python3``
              - Used to link against the C-Python API
            * - `pybind11 <https://pybind11.readthedocs.io/en/stable/>`__
              - **Required**
              - ``brew install pybind11``
              - Heavily used to automatically create the binding code between SOFA and python
            * - `SOFA Framework <https://www.sofa-framework.org/community/doc/>`__
              - **Required**
              - See below for more information
              -

    .. tab-container:: tab3
        :title: Windows

        .. list-table::
            :widths: 15 10 30 45
            :header-rows: 1

            * - Package
              - Type
              - Installation
              - Description
            * - Python 3 libs
              - **Required**
              - Install the latest Python 3 from `here <https://www.python.org/downloads/windows/>`__
              - Used to link against the C-Python API
            * - `pybind11 <https://pybind11.readthedocs.io/en/stable/>`__
              - **Required**
              - ``brew install pybind11``
              - Heavily used to automatically create the binding code between SOFA and python
            * - `SOFA Framework <https://www.sofa-framework.org/community/doc/>`__
              - **Required**
              - See below for correctly linking SOFA when building out-of-tree.
              -

        .. warning::

            Add the installation path of each of those dependencies into your environment variable. For example,
            using the PowerShell of Windows:

            .. code-block:: powershell

                PS $env:Path += ";C:\sofa;C:\pybind11;C:\python"

In-tree build
-------------

We use here the term *in-tree build* to indicate that we wish to compile SofaPython3 in the same build tree as SOFA, which is,
compiling SOFA will automatically compile SofaPython3 as an internal subproject.

Prerequisites - Sofa
--------------------

If you want to compile the SofaPython3 plugin out-of-tree, you will need to either:

* [**Method 1**] Install the `SOFA binaries and headers <https://www.sofa-framework.org/download/>`__
  somewhere on your computer and note its installation directory. Or,
* [**Method 2**] Compile it following the `SOFA build documentation <https://www.sofa-framework.org/community/doc/getting-started/build/linux/>`__.
  Once it is built, execute the installation by going into the build directory of SOFA (for example,
  */home/user/sofa/build/master/*), and using the command ``cmake --install .``

.. warning::

    If you choose the first method (installing the binaries), make sure you have all the SOFA dependencies installed on
    your system (e.g. boost, Eigen3, etc.). You can follow the same instructions as if you had chosen the second method
    (compiling SOFA), but skipping the compilation part.

Once done, export the installation path of SOFA inside the ``SOFA_ROOT`` environment variable. For example,

.. code-block:: bash

    $ export SOFA_INSTALL="/home/user/sofa/build/master/install"


.. note::

   To make sure your ``SOFA_INSTALL`` is well defined, you can verify that the following file path exists:

   .. code-block:: bash

        $SOFA_INSTALL/lib/cmake/SofaFramework/SofaFrameworkTargets.cmake

Install from source
-------------------

Requirement Install
^^^^^^^^^^^^^^^^^^^
- have an up to date version of the sofa source code from the master repository
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

This plugin should compile with out-of-tree builds.



