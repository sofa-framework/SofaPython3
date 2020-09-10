# -*- coding: utf-8 -*-
"""
Splib library
-------------


.. autosummary::
    :toctree: _autosummary

    splib.animation
    splib.caching
    splib.meshing

"""
import Sofa
import Sofa.Core
import inspect
import functools
import inspect
__all__=["animation", "caching", "meshing"]

def pyType2sofaType(v):
    if isinstance(v, str):
        return "string"
    if isinstance(v, int):
        return "int"
    if isinstance(v, float):
        return "double"
    return None

def FunctionToPrefab(f):
        frameinfo = inspect.getframeinfo(inspect.currentframe().f_back)
        definedloc = (frameinfo.filename, frameinfo.lineno)

        def SofaPrefabF(*args, **kwargs):
            class NodeHook(object):
                    def __init__(self, node):
                        self.node = node

                    def addChild(self, name):
                        return selfnode

                    def __getattr__(self, name):
                        return getattr(self.node, name)

            class InnerSofaPrefab(Sofa.Core.Prefab):
                def __init__(self, name):
                    Sofa.Core.Prefab.__init__(self, name=name)

                def doReInit(self):
                    argnames = inspect.getfullargspec(f).args

                    kkwargs = {}
                    kkwargs[argnames[0]] = self
                    for name in argnames[1:]:
                        kkwargs[name] = self.__data__[name].value
                    f(**kkwargs)

            selfnode = InnerSofaPrefab(name="InnerSofaP")
            selfnode.setDefinitionSourceFileName(definedloc[0])
            selfnode.setDefinitionSourceFilePos(definedloc[1])

            ## retrieve meta data from decorated class:
            selfnode.addData(name="prefabname", value=f.__code__.co_name,
                         type="string", help="The prefab's name", group="Infos")
            selfnode.addData(name="docstring", value=f.__doc__,
                         type="string", help="This prefab's docstring", group="Infos")

            ## Now we retrieve all params passed to the prefab and add them as datafields:
            argnames = inspect.getfullargspec(f).args
            defaults = inspect.getfullargspec(f).defaults

            i = len(argnames) - len(defaults)
            for n in range(0, len(defaults)):
                if argnames[i+n] not in selfnode.__data__:
                    selfnode.addPrefabParameter(name=argnames[i+n], value=defaults[n], type=pyType2sofaType(defaults[n]), help="Undefined")

            selfnode.init()
            return selfnode
        return SofaPrefabF

    
class TypeConversionEngine(Sofa.Core.DataEngine):
    def __init__(self, *args, **kwargs):
        Sofa.Core.DataEngine.__init__(self, *args, **kwargs)
        print(kwargs.get("dstType"))
        self.addData(name="dst", type=kwargs.get("dstType"))
        self.addOutput(self.dst)

    def update(self):
        for i in range(0,len(self.inputs())):
            self.dst.value = self.__getattr__(self.inputs()[i].getName() + "_func")(self.inputs()[-1])

    def addDataConversion(self, d, f):
        data = self.addData(name=d.getOwner().getName() + "_" + d.getName(), value=d)
        self.addInput(data)
        self.__setattr__(data.getName() + "_func", f)
