# -*- coding: utf-8 -*-
"""
Splib library
-------------


.. autosummary::
    :toctree: _autosummary

    splib.animation


"""
import Sofa
import Sofa.Core
import inspect
import functools
import inspect
__all__=["animation"]

def typeHelper(d):
    if isinstance(d, int):
        return "int"
    if isinstance(d, str):
        return "string"
    if isinstance(d, float):
        return "float"
    return None

def SofaPrefab(**deckwargs):
    def init_SofaPrefab(f):
        frameinfo = inspect.getframeinfo(inspect.currentframe().f_back)
        definedloc = (frameinfo.filename, frameinfo.lineno)

        def SofaPrefabF(*args, **kwargs):
            selfnode = args[0].addChild(f.__code__.co_name)

            ## retrieve meta data from decorated class:
            selfnode.addData(name="prefabname", value=f.__code__.co_name,
                         type="string", help="The prefab's name", group="Infos")
            selfnode.addData(name="docstring", value=f.__doc__,
                         type="string", help="This prefab's docstring", group="Infos")

            selfnode.setDefinitionSourceFileName(definedloc[0])
            selfnode.setDefinitionSourceFilePos(definedloc[1])
            args = [selfnode] + list(args)[1:]

            inputCB = []
            for k in deckwargs:
                if k not in selfnode.__data__:
                    value = deckwargs[k] if k not in kwargs else kwargs[k]
                    inputCB.append( selfnode.addData(name=k, value=value, group="Prefab's properties", type=typeHelper(deckwargs[k])) )

            keys = list(kwargs.keys())
            for k in keys:
                if k in selfnode.__data__:
                    selfnode.__data__[k] = kwargs[k]
                    del kwargs[k]

            r = f(*args, **kwargs)
            return selfnode
        return SofaPrefabF
    return init_SofaPrefab

class SofaPrefab2(object):
    def __init__(self, cls):
        frameinfo = inspect.getframeinfo(inspect.currentframe().f_back)
        self.cls = cls
        self.definedloc = (frameinfo.filename, frameinfo.lineno)
        functools.update_wrapper(self, cls) ## REQUIRED IN CLASS DECORATORS to transfer the docstring back to the decorated type

    def __call__(self, *args, **kwargs):
            if len(args) == 0:
                Sofa.Helper.msg_error("SofaPython", """A prefab needs to have at least one un-named argument of Node type""")
                return None

            if not isinstance(args[0], Sofa.Core.Node):
                Sofa.Helper.msg_error("SofaPython", "A prefab needs to have at least one un-named argument of Node type got '"+str(type(args[0]))+"'")
                return None

            selfnode = args[0].addChild("DEFAULTNAME")

            ## retrieve meta data from decorated class:
            selfnode.addData(name="prefabname", value=self.cls.__code__.co_name,
                         type="string", help="The prefab's name", group="Infos")
            selfnode.addData(name="docstring", value=self.cls.__doc__,
                         type="string", help="This prefab's docstring", group="Infos")

            selfnode.setDefinitionSourceFileName(self.definedloc[0])
            selfnode.setDefinitionSourceFilePos(self.definedloc[1])

            args = [selfnode] + list(args)
            retnode = self.cls(*args, **kwargs)

            if retnode is None:
                Sofa.Helper.msg_warning(args[0], """The function '' has the SofaPrefab decorator but does not returns a Node.
                                                  Weird behavior may happen.
                                               """, self.definedloc[0], self.definedloc[1])
                return None
            if not isinstance(retnode, Sofa.Core.Node):
                Sofa.Helper.msg_warning(args[0], """The function '' has the SofaPrefab decorator but does not returns a Node.
                                                  Weird behavior may happen.
                                               """, self.definedloc[0], self.definedloc[1])
                return None
            return retnode

class SofaClassPrefab(object):
    """This decorator is used for a seamless integration of the Sofa.Core.Node interface with the Python classes:

    :param parent: The required parent node for this prefab
    :type parent: Sofa.Core.Node
    :param name: The name of this prefab
    :type name: str

    Example of use:
    -----------------

    @SofaPrefab
    class MyPrefab(Sofa.Core.Node):
        def __init__(self, parent, name, coucou=3, plop=42, kikoo=4):
            print (self.getName())
            self.addChild('plop')


    This decorator performs multiple things for the user:
        - It calls super() on the decorated class to initialize the Sofa.Core.Node base class
        - It adds self to the parent node
        - It retrieves meta data from the script (such as the prefab's source code, docstring, definition line etc.) and stores it as datafields
        - It stores the constructor's parameters (args & kwargs) as datafields in the node under the "Prefab" group
    """
    
    def __init__(self, prefab):
        self.o = prefab
        ## Reveal meta data from the decorated class to the decorator
        functools.update_wrapper(self, prefab)

    def __call__(self, *args, **kwargs):
        ## Instantiate decorated class without calling __init__
        prefab = self.o.__new__(self.o, *args, **kwargs)
        ## initialize Sofa.Core.Node on the decorated class
        Sofa.Core.Node.__init__(prefab, args[1])
        ## Call the initializer of the decorated class
        prefab.__init__(*args, **kwargs)
        
        ## retrieve meta data from decorated class:
        prefab.addData(name="prefabname", value=self.o.__init__.__code__.co_name,
                       type="string", help="The prefab's name", group="Infos")
        prefab.addData(name="sourcecode", value=inspect.getsource(self.o),
                       type="string", help="The source code of this prefab", group="Infos")
        prefab.addData(name="docstring", value=self.o.__doc__,
                       type="string", help="This prefab's docstring", group="Infos")

        ## Now we retrieve all params passed to the prefab and add them as datafields:
        argnames = inspect.getfullargspec(prefab.__init__).args
        defaults = inspect.getfullargspec(prefab.__init__).defaults
        for i in range(0, len(argnames) - len(defaults) - 1):
            defaults = ("",) + defaults

        i = -1
        for n in argnames:
            if n == "self": continue
            i += 1
            
            if n == "parent":
                ## If there's no parent passed, raise an exception.
                if i > len(args) or type(args[i]) != Sofa.Core.Node:
                    print(str(args[i]))
                    raise AttributeError("The 'parent' attribute is mandatory for prefabs,"
                                         " and was not provided")
                ## otherwise add the prefab to parent
                args[i].addChild(prefab)
                continue
                
            ## Don't try to store prefab params that already are datafields (such as name)
            hasField = False
            for f in prefab.getDataFields():
                if n == f.getName():
                    hasField = True
                    break
            if hasField:
                continue
            prefab.addData(name=n, value=str(args[i] if i < len(args) else defaults[i]), type="string", group="Prefab")

        ## TODO(@bmarques): Retrieve kwargs the same way & handle name conflicts, if any
            
        ## Return the constructed object:
        return prefab
