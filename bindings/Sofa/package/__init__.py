"""
All the key components supported by the sofa-consortium

Example of use:
  .. code-block:: python

    import Sofa.Core
    import Sofa.Simulation
    import SofaRuntime
    SofaRuntime.importPlugin("Sofa.Component")

    n = Sofa.Core.Node("MyNode")
    n.addChild("Node2")
    n.addObject("MechanicalObject", name="dofs")

    Sofa.Simulation.init(n)
    Sofa.Simulation.print(n)

.. rubric:: Sub modules
.. autosummary::
        :toctree: generated/Sofa
        :template: autosummary/package.rst
        :recursive:

        Core
        Simulation
        Helper
        Types
        Gui

.. rubric:: Functions
.. autosummary::
      :toctree: generated/modules/Sofa

      msg_deprecated
      msg_warning
      msg_error
      msg_info

"""

import sys
import os
import inspect
import functools
import traceback
import importlib

print("---------------------------------------")
print("Checking SOFA_ROOT and SOFAPYTHON3_ROOT")

# check if SOFA_ROOT has been (well) set
sofa_root = os.environ.get('SOFA_ROOT')
if sofa_root:
    print("Using environment variable SOFA_ROOT: " + sofa_root)
else:
    print("Warning: environment variable SOFA_ROOT is empty. Trying to guess it.")
    # try a guess from <sofa_root>/plugins/SofaPython3/lib/python3/site-packages/Sofa
    sofa_root_guess = os.path.abspath(os.path.dirname(os.path.realpath(__file__)) + '/../../../../../..')
    if os.path.isdir(os.path.abspath(sofa_root_guess + '/lib' )):
        print("Guessed SOFA_ROOT: " + sofa_root_guess)
        sofa_root = sofa_root_guess
        os.environ["SOFA_ROOT"] = sofa_root
    else:
        print("Warning: cannot guess SOFA_ROOT",
              "Loading SOFA libraries will likely fail and/or SOFA won't find its resources.")

if sofa_root and sys.platform == 'win32':

    # check if SOFAPYTHON3_ROOT has been (well) set, only useful for Windows
    sofapython3_root = os.environ.get('SOFAPYTHON3_ROOT')
    if sofapython3_root:
        print("Using environment variable SOFAPYTHON3_ROOT: " + sofapython3_root)
    else:
        print("Warning: environment variable SOFAPYTHON3_ROOT is empty. Trying to guess it.")
        # try a guess from <sofapython3_root>/lib/python3/site-packages/Sofa
        sofapython3_root_guess = os.path.abspath(os.path.dirname(os.path.realpath(__file__)) + '/../../../..')
        if os.path.isdir(os.path.abspath(sofapython3_root_guess + '/lib' )):
            print("Guessed SOFAPYTHON3_ROOT: " + sofapython3_root_guess)
            sofapython3_root = sofapython3_root_guess
            os.environ["SOFAPYTHON3_ROOT"] = sofapython3_root
        else:
            print("Warning: cannot guess SOFAPYTHON3_ROOT",
                  "Loading SofaPython3 modules will likely fail.")

    # Windows-only: starting from python 3.8, python wont read the env. variable PATH to get SOFA's dlls.
    # os.add_dll_directory() is the new way to add paths for python to get external libraries.
    sofa_bin_path = os.path.join(sofa_root, "bin")
    sofapython3_bin_path = os.path.join(sofapython3_root, "bin")

    # A user using a build configuration could have a multiple-configuration type build
    # which is typical on Windows and MSVC; and MacOS with XCode
    # If the user set the env.var SOFA_BUILD_CONFIGURATION, he can choose a preferred configuration.
    # If it is not found, it is considered as an error.
    sofa_build_configuration = os.environ.get('SOFA_BUILD_CONFIGURATION')
    compilation_modes = []
    if sofa_build_configuration:
        print("SOFA_BUILD_CONFIGURATION is set to " + sofa_build_configuration)
        compilation_modes = [sofa_build_configuration]
    else:
        compilation_modes = ["Release", "RelWithDebInfo", "Debug", "MinSizeRel"] # Standard multi-configuration modes in CMake

    sofa_bin_compilation_modes = []
    sofapython3_bin_compilation_modes = []
    for mode in compilation_modes:
        if os.path.isdir(os.path.abspath(os.path.join(sofa_bin_path, mode))):
            sofa_bin_compilation_modes.append(os.path.join(sofa_bin_path, mode))
        if os.path.isdir(os.path.abspath(os.path.join(sofapython3_bin_path, mode))):
            sofapython3_bin_compilation_modes.append(os.path.join(sofapython3_bin_path, mode))

    if sofa_bin_compilation_modes:
        print("Detected SOFA development build")
    if sofapython3_bin_compilation_modes:
        print("Detected SofaPython3 development build")

    sofa_bin_candidates = [sofa_bin_path] + sofa_bin_compilation_modes
    sofapython3_bin_candidates = [sofapython3_bin_path] + sofapython3_bin_compilation_modes

    sofa_helper_dll = ["Sofa.Helper.dll", "Sofa.Helper_d.dll"]
    sofa_file_test = ""
    for candidate in sofa_bin_candidates:
        for dll in sofa_helper_dll:
            sofa_file_test = os.path.join(candidate, dll)
            if os.path.isfile(sofa_file_test):
                print(f"Found {dll} in {candidate}")
                sofa_bin_path = candidate
                break
        else:
            continue
        break

    sofa_python3_dll = ["SofaPython3.dll", "SofaPython3_d.dll"]

    sofapython3_file_test = ""
    for candidate in sofapython3_bin_candidates:
        for dll in sofa_python3_dll:
            sofapython3_file_test = os.path.join(candidate, dll)
            if os.path.isfile(sofapython3_file_test):
                print(f"Found {dll} in {candidate}")
                sofapython3_bin_path = candidate
                break
        else:
            continue
        break

    if not os.path.isfile(sofa_file_test):
        print("Warning: environment variable SOFA_ROOT is set but seems invalid.",
              "Loading SOFA libraries will likely fail.")
        print("SOFA_ROOT is currently: " + sofa_root)
    if not os.path.isfile(sofapython3_file_test):
        print("Warning: cannot find SofaPython3.dll at path: " + sofapython3_bin_path)
        print("This path will NOT be added to the DLL search path.",
              "Loading SofaPython3 python modules will likely fail.")

    if sys.version_info.minor >= 8:
        # Starting from python3.8 we need to explicitly find SOFA libraries
        if os.path.isfile(sofa_file_test):
            os.add_dll_directory(sofa_bin_path)
        if os.path.isfile(sofapython3_file_test):
            os.add_dll_directory(sofapython3_bin_path)
    else:
        # Add temporarily the bin/lib path to the env variable PATH
        if os.path.isfile(sofa_file_test):
            os.environ['PATH'] = sofa_bin_path + os.pathsep + os.environ['PATH']
        if os.path.isfile(sofapython3_file_test):
            os.environ['PATH'] = sofapython3_bin_path + os.pathsep + os.environ['PATH']

print("---------------------------------------")
sys.stdout.flush()

import Sofa.constants
import Sofa.Helper
import Sofa.Core
import Sofa.Simulation
import Sofa.Types
import Sofa.Components
import SofaTypes

from .prefab import *

__all__=["constants", "Helper", "Core", "Simulation", "Types", "SofaTypes", "prefab"]

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
    ss='At: '
    for entry in o:
        ss+= '\n  '+ str(entry[1]) + '(' + str(entry[2]) + '): '+ entry[3]
    return ss + "\n"

def getStackForSofa():
    """returns the current stack with a "informal" formatting. """
    ## we exclude the first level in the stack because it is the getStackForSofa() function itself.
    ss=inspect.stack()[1:]
    return formatStackForSofa(ss)


def getPythonCallingPointAsString():
    """returns the last entry with an "informal" formatting. """

    ## we exclude the first level in the stack because it is the getStackForSofa() function itself.
    ss=inspect.stack()[1:2]
    return formatStackForSofa(ss)


def getPythonCallingPoint():
    """returns the tuple with closest filename & line. """
    ## we exclude the first level in the stack because it is the getStackForSofa() function itself.
    ss=inspect.stack()[1:2]
    tmp=(os.path.abspath(ss[1]), ss[2])
    return tmp


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

    return repr(value)+"\n"+s


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

def pyType2sofaType(v):
    if isinstance(v, bool):
        return "bool"
    if isinstance(v, str):
        return "string"
    if isinstance(v, int):
        return "int"
    if isinstance(v, float):
        return "double"
    if isinstance(v, list) and len(v)==3:
        return "Vec3d"
    if isinstance(v, list):
        return "vector<double>"
    if isinstance(v, Sofa.PyTypes.DataType):
        return v.sofaTypeName
    return None


def msg_error(target, message):
    frameinfo = inspect.getframeinfo(inspect.currentframe().f_back)
    Sofa.Helper.msg_error(target, message, frameinfo.filename, frameinfo.lineno)

def msg_info(target, message):
    frameinfo = inspect.getframeinfo(inspect.currentframe().f_back)
    Sofa.Helper.msg_info(target, message, frameinfo.filename, frameinfo.lineno)

def msg_warning(target, message):
    frameinfo = inspect.getframeinfo(inspect.currentframe().f_back)
    Sofa.Helper.msg_warning(target, message, frameinfo.filename, frameinfo.lineno)

def msg_deprecated(target, message):
    frameinfo = inspect.getframeinfo(inspect.currentframe().f_back)
    Sofa.Helper.msg_deprecated(target, message, frameinfo.filename, frameinfo.lineno)

import inspect
def PrefabBuilder(f):
    frameinfo = inspect.getframeinfo(inspect.currentframe().f_back)
    definedloc = (frameinfo.filename, frameinfo.lineno)

    def SofaPrefabF(*args, **kwargs):
        class InnerSofaPrefab(Sofa.Core.RawPrefab):
            def __init__(self, *args, **kwargs):
                Sofa.Core.RawPrefab.__init__(self, *args, **kwargs)
                self.isValid = True

            def doReInit(self):
                if not self.isValid:
                    return
                try:
                    argnames = inspect.getfullargspec(f).args

                    kkwargs = {}
                    kkwargs["self"] = self
                    for name in argnames[:]:
                        if name != "self":
                            kkwargs[name] = self.__data__[name].value

                    self.cb(**kkwargs)
                except Exception as e:
                    self.isValid = False
                    exc_type, exc_value, exc_tb = sys.exc_info()
                    Sofa.Helper.msg_error(self, "Unable to build prefab  \n  "+getSofaFormattedStringFromException(e))
        try:
            selfnode = None
            kwargs["name"] = kwargs.get("name", f.__code__.co_name)
            selfnode = InnerSofaPrefab(*args, **kwargs)
            selfnode.setDefinitionSourceFileName(definedloc[0])
            selfnode.setDefinitionSourceFilePos(definedloc[1])
            selfnode.setSourceTracking(definedloc[0])
            selfnode.cb = f
            ## retrieve meta data from decorated class:
            selfnode.addData(name="prefabname", value=f.__code__.co_name,
                             type="string", help="The prefab's name", group="Infos")
            selfnode.addData(name="docstring", value=f.__doc__,
                             type="string", help="This prefab's docstring", group="Infos")

            ## Now we retrieve all params passed to the prefab and add them as datafields:
            argnames = inspect.getfullargspec(f).args
            defaults = inspect.getfullargspec(f).defaults

            if argnames is None:
                argnames = []
                defaults = []

            if defaults is None:
                defaults = []

            i = len(argnames) - len(defaults)
            for n in range(0, len(defaults)):
                if argnames[i+n] not in selfnode.__data__:
                    if pyType2sofaType(defaults[n]) != None:
                        selfnode.addPrefabParameter(name=argnames[i+n],
                                                    default=kwargs.get(argnames[i+n], defaults[n]),
                                                    type=pyType2sofaType(defaults[n]), help="Undefined")
                    else:
                        Sofa.Helper.msg_error("Missing type for parameters: ", argnames[i+n])
            selfnode.init()

        except Exception as e:
            if selfnode is not None:
                selfnode.isValid=False
                Sofa.Helper.msg_error(selfnode, "Unable to create prefab because: "+getSofaFormattedStringFromException(e))
            else:
                Sofa.Helper.msg_error("PrefabBuilder", "Unable to create prefab because: "+getSofaFormattedStringFromException(e))
        return selfnode
    SofaPrefabF.__dict__["__original__"] = f
    return SofaPrefabF

def import_sofa_python_scene(path_to_scene : str):
    """Return a python module containing a SOFA scene"""
    spec_from_location = importlib.util.spec_from_file_location("sofa.scene", path_to_scene)
    module_name = importlib.util.module_from_spec(spec_from_location)
    sys.modules["module.name"] = module_name
    spec_from_location.loader.exec_module(module_name)

    if not hasattr(foo, "createScene"):
        raise Exception("Unable to find 'createScene' in module "+path_to_scene)

    return foo

