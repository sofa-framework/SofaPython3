#######
Install
#######

The SofaPython3 plugins allows you to embed a python3 interpreter into an existing SOFA application (eg: runSofa) and to create/launch SOFA simulations from a python environment.
Depending on your use case you may have different step to follow to get a fully working SofaPython3 environnement.

.. _getsofapython3:

***************
Get SofaPython3
***************

Latest version of the SofaPython3 plugins are now shipped within the binary SOFA release supported by the SOFA Constortium.
All you need is therefore to download these SOFA binaries from the `SOFA website <https://www.sofa-framework.org/download/>`_.


********************
Get python installed
********************

First, make sure you have the same version of python installed on your computer as the one that is used in the binary version.

.. tabs::

        .. tab:: Ubuntu

			Run in a terminal:

			.. code-block:: bash

				sudo add-apt-repository ppa:deadsnakes/ppa
				sudo apt install libpython3.12 python3.12 python3-pip
				python3.12 -m pip install numpy

			if you want to launch the runSofa:

			.. code-block:: bash

				sudo apt install libopengl0


        .. tab:: MacOS

			Run in a terminal:

			.. code-block:: bash

				brew install python@3.12
				export PATH="/usr/local/opt/python@3.12/bin/:$PATH"

			BigSur only:

			.. code-block:: bash

				pip3 install --upgrade pip
				python3.12 -m pip install numpy

			Catalina only:

			.. code-block:: bash

				pip3 install numpy


        .. tab:: Windows

		Download and install `Python 3.12 64bit <https://www.python.org/ftp/python/3.12.1/python-3.12.1-amd64.exe>`_


**********************
Setup your environment
**********************

using runSofa
*************

Using SofaPython3 in runSofa requires loading the SofaPython3 plugin in your runSofa environment. If you downloaded and installed SOFA from the SOFA website (as explained above, see :ref:`getsofapython3`), you can load the SofaPython3 plugin using the PluginManager (in the GUI) or by auto-loading the plugin in runSofa: simply copy the file **plugin_list.conf.default** in *<SOFA_build>/lib*, and rename it **plugin_list.conf**, then add the line:

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


.. tabs::

        .. tab:: Ubuntu

			Run in a terminal:

			.. code-block:: bash

				export SOFA_ROOT=/path/to/SOFA_install
				export PYTHONPATH=/path/to/SofaPython3/lib/python3/site-packages:$PYTHONPATH

        .. tab:: MacOS

			Run in a terminal:

			.. code-block:: bash

				export SOFA_ROOT=/path/to/SOFA_install
				export PYTHONPATH=/path/to/SofaPython3/lib/python3/site-packages:$PYTHONPATH
				export PATH="/usr/local/opt/python@3.12/bin/:$PATH"


        .. tab:: Windows

		    * Create a system variable **SOFA_ROOT** and set it to ``<SOFA-install-directory>``
		    * Create a system variable **PYTHON_ROOT** and set it to ``<Python3-install-directory>``
		    * Create a system variable **PYTHONPATH** and set it to ``%SOFA_ROOT%\plugins\SofaPython3\lib\python3\site-packages``
		    * Edit the system variable **Path** and add at the end ``;%PYTHON_ROOT%;%PYTHON_ROOT%\DLLs;%PYTHON_ROOT%\Lib;%SOFA_ROOT%\bin;``
		    * Open a Console (cmd.exe) and run ``python -V && python -m pip install numpy scipy``

		After that, all you need to do is open a Console (cmd.exe) and run: ``runSofa -lSofaPython3``


⚠️ It must be noted that depending on the plugins you use, you might have to add the *site-packages/* paths associated to these plugins to your ``PYTHONPATH``.
These are usually located in */path_to_plugin/lib/python3/site-packages*.

To discover, how to use SOFA in any python3 interpreter, please refer to the associated `FirstSteps section <https://sofapython3.readthedocs.io/en/latest/content/FirstSteps.html#with-the-python3-interpreter>`_


Get support
***********

🙋 To freely get technical assistance from the community, please do not hesitate to join the `SofaPython3 GitHub forum <https://github.com/sofa-framework/sofapython3/discussions>`_ and post there any question related to SofaPython3.

👨‍🏫 To quickly level up on SOFA, never hesitate to request `SOFA training sessions <https://www.sofa-framework.org/sofa-events/sofa-training-sessions/>`_.
