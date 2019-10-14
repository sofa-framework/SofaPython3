"""
Package containg the binding for the core of Sofa
-------------------------------------------------

Example of use:
  .. code-block:: python

    import Sofa.Core
    import Sofa.Simulation
    import SofaRuntime
    SofaRuntime.importPlugin("SofaAllCommonComponents")

    n = Sofa.Core.Node("MyNode")
    n.addChild("Node2")
    n.addObject("MechanicalObject", name="dofs")

    Sofa.Simulation.init(n)
    Sofa.Simulation.print(n)

Submodules:
  .. autosummary::
    :toctree: _autosummary

    Sofa.Core
    Sofa.Simulation
    Sofa.Types
    Sofa.Helper
"""

import sys
import os
import inspect
import traceback
import Sofa.Helper
import importlib

# Keep a list of the modules always imported in the Sofa-PythonEnvironment
try:
    __SofaPythonEnvironment_importedModules__
except:
    __SofaPythonEnvironment_importedModules__ = sys.modules.copy()

    # some modules could be added here manually and can be modified procedurally
    # e.g. plugin's modules defined from c++
    __SofaPythonEnvironment_modulesExcludedFromReload = []


def unloadModules():
    """ call this function to unload python modules and to force their reload
        (useful to take into account their eventual modifications since
        their last import).
    """
    global __SofaPythonEnvironment_importedModules__
    toremove = [name for name in sys.modules if not name in __SofaPythonEnvironment_importedModules__ and not name in __SofaPythonEnvironment_modulesExcludedFromReload ]
    for name in toremove:
        del(sys.modules[name]) # unload it


def formatStackForSofa(o):
    """ format the stack trace provided as a parameter into a string like that:
        in filename.py:10:functioname()
          -> the line of code.
        in filename2.py:101:functioname1()
            -> the line of code.
        in filename3.py:103:functioname2()
              -> the line of code.
    """
    ss='Python Stack: \n'
    for entry in o:
        ss+= ' in ' + str(entry[1]) + ':' + str(entry[2]) + ':'+ entry[3] + '()  \n'
        ss+= '  -> '+ entry[4][0] + '  \n'
        return ss


def getStackForSofa():
    """returns the current stack with a "informal" formatting. """
    ## we exclude the first level in the stack because it is the getStackForSofa() function itself.
    ss=inspect.stack()[1:]
    return formatStackForSofa(ss)


def getPythonCallingPointAsString():
    """returns the last entry with an "informal" formatting. """

    ## we exclude the first level in the stack because it is the getStackForSofa() function itself.
    ss=inspect.stack()[-1:]
    return formatStackForSofa(ss)


def getPythonCallingPoint():
    """returns the tupe with closest filename & line. """
    ## we exclude the first level in the stack because it is the getStackForSofa() function itself.
    ss=inspect.stack()[1]
    tmp=(os.path.abspath(ss[1]), ss[2])
    return tmp


# returns a dictionary of all callable objects in the module, with their type as key
def getPythonModuleContent(moduledir, modulename):
#    Sofa.msg_info("PythonAsset LOADING module " + modulename + " in " + moduledir)
    objects = {}
    # First let's load that script:
    try:
        sys.path.append(moduledir)

        if modulename in sys.modules:
            del(sys.modules[modulename])
        m = importlib.import_module(modulename)
    except ImportError(e):
        print ("PythonAsset ERROR: could not import module " + modulename)
        print (e)
        return objects
    except Exception(e):
        print ("Exception: in " + modulename + ":\n" + str(e))
        return objects

    # module loaded, let's see what's inside:
#    Sofa.msg_info("Module Loaded, let's see what's inside...")
    if "createScene" in dir(m):
        # print("We found a createScene entry point, let's load it")
        objects["createScene"] = { "type":"function", "docstring": m.createScene.__doc__ }
    for i in dir(m):
        if i == "SofaObject" or i == "SofaPrefab" or inspect.isbuiltin(i) or not callable(getattr(m, i)):
            continue
        class_ = getattr(m, i)
        if inspect.getmodule(class_).__file__ != m.__file__:
            continue

        docstring = str(class_.__doc__) if class_.__doc__ != None else "Undocumented prefab"
        if inspect.isclass(eval("m." + i)):
            # A non-decorated class
            if issubclass(eval("m." + i), Sofa.PythonScriptController):
                objects[i] = { "type":"PythonScriptController", "docstring": docstring }
            elif issubclass(eval("m." + i), Sofa.PythonScriptDataEngine):
                objects[i] = { "type":"PythonScriptDataEngine", "docstring": docstring }
            else:
                objects[i] = { "type":"Class", "docstring": docstring }
        else:
            # a class decorated with @SofaPrefab:
            if class_.__class__.__name__ == "SofaPrefab" \
               and i != "SofaPrefab":
                objects[i] = { "type":"SofaPrefab", "docstring": docstring }
            else:
                objects[i] = { "type":"function", "docstring": docstring }
                #    Sofa.msg_info(str(objects))
    return objects


import code
def getPythonModuleDocstring(mpath):
    "Get module-level docstring of Python module at mpath, e.g. 'path/to/file.py'."
    print(mpath)
    co = compile(open(mpath).read(), mpath, 'exec')
    if co.co_consts and isinstance(co.co_consts[0], basestring):
        docstring = co.co_consts[0]
    else:
        docstring = ""
    return str(docstring)


def sendMessageFromException(e):
    exc_type, exc_value, exc_tb = sys.exc_info()
    sofaExceptHandler(exc_type, exc_value, exc_tb)


def sofaFormatHandler(type, value, tb):
    global oldexcepthook
    """This exception handler, convert python exceptions & traceback into more classical sofa error messages of the form:
       Message Description
       Python Stack (most recent are at the end)
          File file1.py line 4  ...
          File file1.py line 10 ...
          File file1.py line 40 ...
          File file1.py line 23 ...
            faulty line
    """
    s="\nPython Stack (most recent are at the end): \n"
    for line in traceback.format_tb(tb):
        s += line

    return repr(value)+" "+s


def getSofaFormattedStringFromException(e):
    exc_type, exc_value, exc_tb = sys.exc_info()
    return sofaFormatHandler(exc_type, exc_value, exc_tb)

def sofaExceptHandler(type, value, tb):
    global oldexcepthook
    """This exception handler, convert python exceptions & traceback into more classical sofa error messages of the form:
       Message Description
       Python Stack (most recent are at the end)
          File file1.py line 4  ...
          File file1.py line 10 ...
          File file1.py line 40 ...
          File file1.py line 23 ...
            faulty line
    """
    h = type.__name__

    if str(value) != '':
        h += ': ' + str(value)
    
    s = ''.join(traceback.format_tb(tb))
    
    Sofa.Helper.msg_error(h + '\n' + s, "line", 7)

sys.excepthook=sofaExceptHandler

