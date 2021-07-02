Compilation
===========

The compilation of the SofaPython3 can be done automatically when `building SOFA <https://www.sofa-framework.org/community/doc/getting-started/build/linux/>`_,
or it can be done independently using an already compiled/installed SOFA:

* the former is called an *in-tree build*, which is, the compilation of SofaPython3 is included inside the build tree of SOFA
* the latter is called an *out-of-tree build*, hence the compilation of SofaPython3 is done in its own build tree, usually placed somewhere else than SOFA. 



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
              - You can either `download it <https://www.sofa-framework.org/download/>`__, or
                `compile it <https://www.sofa-framework.org/community/doc/getting-started/build/linux/>`__ from sources
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
              - You can either `download it <https://www.sofa-framework.org/download/>`__, or
                `compile it <https://www.sofa-framework.org/community/doc/getting-started/build/macos/>`__ from sources
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
              - Compile and install pybind11 from `sources <https://github.com/pybind/pybind11>`__
              - Heavily used to automatically create the binding code between SOFA and python
            * - `SOFA Framework <https://www.sofa-framework.org/community/doc/>`__
              - **Required**
              - You can either `download it <https://www.sofa-framework.org/download/>`__, or
                `compile it <https://www.sofa-framework.org/community/doc/getting-started/build/windows/>`__ from sources
              -

        .. warning::

            Add the installation path of each of those dependencies into your environment variable. For example,
            using the PowerShell of Windows:

            .. code-block:: powershell

                PS $env:Path += ";C:\sofa;C:\pybind11;C:\python"




In-tree build
-------------

We use here the term *in-tree build* to indicate that we wish to compile SofaPython3 in the same build tree as SOFA, which is,
compiling SOFA will automatically compile SofaPython3 as an internal subproject. In this case, the compilation of the
plugin should be straightforward, given that you installed correctly all the dependencies of SofaPython3.

Follow the same steps as the one needed to compile SOFA : `<https://www.sofa-framework.org/community/doc/getting-started/build/linux>`__.
When you get to the step **Generate a Makefile with CMake**, activate the CMake option **SOFA_FETCH_SOFAPYTHON3**:

.. figure:: /images/cmake_fetch_sp3.jpg
    :alt: Activate the cmake option SOFA_FETCH_SOFAPYTHON3
    :align: center

and configure CMake. This will pull the source code of SofaPython3 into the `application/plugins` directory of SOFA's source directory.

.. note::

    The CMake option **SOFA_FETCH_SOFAPYTHON3** should disable itself automatically once CMake has been configure one
    time. This is to prevent pulling again the SofaPython3 source code again the next time you run cmake. If you need
    to update the plugin, you can manually turn the option `ON` again to pull the latest changes made in SofaPython3.

Once cmake has pulled the source code of SofaPython3 into the `application/plugins` directory, a new CMake option should
now be available : **PLUGIN_SOFAPYTHON3**. Make sure you activate it and re-run CMake configure and generate:

.. figure:: /images/cmake_activate_sp3.jpg
    :alt: Activate the cmake option PLUGIN_SOFAPYTHON3
    :align: center

If everything went fine during the cmake configuration stage, you should be able to find some information on the version
of python and pybind11 used for the compilation of the plugin:

.. code-block:: bash

    $ cmake ..
    (...)
    Adding plugin SofaPython3
    -- Python:
        Version: 3.9.1
        Executable: /usr/bin/python3.9
        Headers: /usr/include/python3.9
        Libraries: /usr/lib64/libpython3.9.so
        User site: /home/jnbrunet/.local/lib/python3.9/site-packages
    -- pybind11:
        Version: 2.6.1
        Config: /usr/share/cmake/pybind11/pybind11Config.cmake
    (...)
    -- Configuring done
    -- Generating done
    -- Build files have been written to: /home/jnbrunet/sources/sofa/build

At this point, you are ready to start the compilation of SOFA and the SofaPython3 plugin.



Out-of-tree build
-----------------

As detailed above in the `Prerequisite <https://sofapython3.readthedocs.io/en/latest/menu/Compilation.html#prerequisites>`_ section, you must have an installed version of SOFA in order to compile the SofaPython3 plugin *out-of-tree*. Not only must the sources be compiled following the SOFA documentation, but you have to make sure to have an **install** directory, i.e. to execute the installation (e.g. by runnin the command ``cmake --install .`` from the SOFA build directory)

Once done, export the installation path of SOFA inside the ``SOFA_ROOT`` environment variable. For example,

.. code-block:: bash

    $ export SOFA_ROOT="/home/user/sofa/build/master/install"


.. note::

   To make sure your ``SOFA_ROOT`` is well defined, you can verify that the following file path exists:

   .. code-block:: bash

        $SOFA_ROOT/lib/cmake/SofaFramework/SofaFrameworkTargets.cmake

We are now ready to compile SofaPython3. Let's first create three new environment variables: **SP3_SRC**, **SP3_BUILD**
and **SP3_ROOT** that will be use to set the location to the source code, the built files and the installed files,
respectively. For example:

.. code-block:: bash

    $ export SP3_SRC=/opt/SofaPython3/src
    $ export SP3_BUILD=/opt/SofaPython3/build
    $ export SP3_ROOT=/opt/SofaPython3/build/install

Let's now pull the source code of the plugin, configure the build and start the compilation and installation.

.. code-block:: bash

    $ git clone https://github.com/sofa-framework/SofaPython3.git $SP3_SRC
    $ cmake -DCMAKE_PREFIX_PATH=$SOFA_ROOT/lib/cmake \
            -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_INSTALL_PREFIX=$SP3_ROOT\
            -S $SP3_SRC \
            -B $SP3_BUILD
    $ cmake --build $SP3_BUILD
    $ cmake --install $SP3_BUILD

That's it, you are now ready to use SofaPython3. Keep the environment variable **SOFA_ROOT** and **SP3_ROOT** close by
for the upcoming sections.