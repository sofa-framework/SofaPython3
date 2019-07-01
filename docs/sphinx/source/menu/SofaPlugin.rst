The SofaPython3 plugin
================================

The SofaPython3 plugins embeds a python interpreter into a Sofa simulation. 

Using SofaPyton3 in runSofa:
-----------

    Using SofaPython3 in runSofa requires loading the SofaPython3 plugin in your runSofa environment.

    You can do this by loading the plugin using the PluginManager (in the GUI) or by auto-loading the plugin in runSofa: simply add the following line in your <SOFA_build>/lib/plugin_list.conf.default:

.. code-block:: bash
	SofaPython3 NO_VERSION


    Having the SofaPython3 plugin active will allow you to open scene files using the ".py, .py3, .pyscn, .pyscn3" file extension in runSofa

Create a SOFA scene using the SofaPython3 plugin:
----------

    Just like with the legacy SofaPython plugin, SofaPython3 searches for a `createScene(arg0: Sofa.Node) -> Sofa.Node` method in the python script, to use as the entry point of the SOFA simulation. Thus define this method:

.. code-block:: python
	def createScene(root):
		root.name = "root"
		root.dt = 0.01
		root.gravity = [0, -9.81, 0]
		...

        You can then load a python scene (pyscn3, py3) in Sofa.  
        
        
For more information on how to use the SOFA modules bindings in python, visit this page: :doc:`SofaModule`
