#######
Install
#######

The SofaPython3 plugins allows you to embed a python3 interpreter into an existing SOFA application (eg: runSofa) and to create/launch SOFA simulations from a python environment.
Depending on your use case you may have different step to follow to get a fully working SofaPython3 environnement.

***************
Get SofaPython3
***************

Latest version of the SofaPython3 plugins are now shipped directly by the Sofa-Constortium directly from the `Sofa website <https://www.sofa-framework.org/download/>`


*****
Setup
*****

First make sure you have the same version of python installed on your computer as the one that is used in the binary version.

.. content-tabs::

	.. tab-container:: tab1
		:title: Ubuntu

			Run in a terminal:

			.. code-block:: bash

				sudo add-apt-repository ppa:deadsnakes/ppa
				sudo apt install libpython3.8 python3.8 python3-pip
				python3.8 -m pip install numpy

			if you want to launch the runSofa:

			.. code-block:: bash

				sudo apt install libopengl0


	.. tab-container:: tab2
		:title: MacOS

			Run in a terminal:

			.. code-block:: bash

				brew install python@3.8
				export PATH="/usr/local/opt/python@3.8/bin/:$PATH"

			BigSur only:

			.. code-block:: bash

				pip3 install --upgrade pip
				python3.8 -m pip install numpy

			Catalina only:

			.. code-block:: bash

				pip3 install numpy


	.. tab-container:: tab3
		:title: Windows

		Download and install `Python 3.8 64bit <https://www.python.org/ftp/python/3.8.10/python-3.8.10-amd64.exe>`_


***************************
Check everything is correct
***************************

using runSofa
*************

Using SofaPython3 in runSofa requires loading the SofaPython3 plugin in your runSofa environment.
    

* If you downloaded and installed SOFA and its headers from the `SOFA website <https://www.sofa-framework.org/download/>`_, or if you `compiled SofaPython3 in-tree <https://sofapython3.readthedocs.io/en/latest/content/Compilation.html#in-tree-build>`_, you can load the SofaPython3 plugin using the PluginManager (in the GUI) or by auto-loading the plugin in runSofa: simply copy the file **plugin_list.conf.default** in *<SOFA_build>/lib*, and rename it **plugin_list.conf**, then add the line:

	.. code-block:: text 

		SofaPython3 NO_VERSION

	..
		Note that adding the line to the file **plugin_list.conf.default** in *<SOFA_build>/lib* would work, but you would need to add the line everytime you compile the code.

	Having the SofaPython3 plugin active will allow you to open scene files using the ".py, .py3, .pyscn, .pyscn3" file extension in runSofa, with the command :

	.. code-block:: bash

		<SOFA_build>/bin/runSofa <your_python_file>


using python3
*************

Before running your simulations, you must make sure to define the following environment variables:


.. content-tabs::

	.. tab-container:: tab1
		:title: Ubuntu

			Run in a terminal:

			.. code-block:: bash

				export SOFA_ROOT=/path/to/SOFA_install
				export PYTHONPATH=/path/to/SofaPython3/lib/python3/site-packages:$PYTHONPATH

	.. tab-container:: tab2
		:title: MacOS

			Run in a terminal:

			.. code-block:: bash

				export SOFA_ROOT=/path/to/SOFA_install
				export PYTHONPATH=/path/to/SofaPython3/lib/python3/site-packages:$PYTHONPATH
				export PATH="/usr/local/opt/python@3.8/bin/:$PATH"


	.. tab-container:: tab3
		:title: Windows

		    * Create a system variable **SOFA_ROOT** and set it to ``<SOFA-install-directory>``
		    * Create a system variable **PYTHON_ROOT** and set it to ``<Python3-install-directory>``
		    * Create a system variable **PYTHONPATH** and set it to ``%SOFA_ROOT%\plugins\SofaPython3\lib\python3\site-packages``
		    * Edit the system variable **Path** and add at the end ``;%PYTHON_ROOT%;%PYTHON_ROOT%\DLLs;%PYTHON_ROOT%\Lib;%SOFA_ROOT%\bin;``
		    * Open a Console (cmd.exe) and run ``python -V && python -m pip install numpy scipy``

		After that, all you need to do is open a Console (cmd.exe) and run: ``runSofa -lSofaPython3``



It is possible to use SOFA in any python3 interpreter.
The following code should cover most basic SOFA elements:

.. code-block:: python

        # to be able to create SOFA objects you need to first load the plugins that implement them.
        # For simplicity you can load the plugin "Sofa.Component" that will load all most
        # common sofa objects.
        import SofaRuntime
        SofaRuntime.importPlugin("Sofa.Component")

        # to create elements like Node or objects
	import Sofa.Core


Get support
***********

To freely get technical assistance from the community, please do not hesitate to join the `SofaPython3 GitHub forum <https://github.com/sofa-framework/sofapython3/discussions>`_ and post there any question related to SofaPython3.

To quickly level up on SOFA, never hesitate to request `SOFA training sessions <https://www.sofa-framework.org/sofa-events/sofa-training-sessions/>`_.
