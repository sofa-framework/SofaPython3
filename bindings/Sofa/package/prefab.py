import Sofa.Core
import inspect

class Prefab(Sofa.Core.RawPrefab):
    """
    The Prefab object is a Base class to create custom Prefab components for SOFA, implemented in python.
    A Prefab can have parameters defined in the "properties" list (datafields under the group "Prefab's Properties"), that trigger its doReInit() function when modified. Parameters must be in a list of dictionaries containing the 3 required fields ("name", "type", "help") and one optional field ("default").
    When initializing a Prefab, if a prefab property name matches a keyword argument fed to the Prefab's initializer, the property will be added to the prefab as a SOFA data of the given type.

    Prefabs have protected keyword arguments:
    - name: the name of the prefab instance
    - parent and parents: can't be used together, they set the context of the prefab, thus allowing paths resolution for Prefab parameters whose arguments are passed as link paths (strings). parents (with an '-s') sets multi-node contexts
    - All other protected keyword arguments in SOFA components    

    Example of use:
      .. code-block:: python
        import Sofa.Core

        class Foo(Sofa.Core.Prefab):
            properties = [{ 'name': 'n', 'type': 'int', 'help': 'number of times this prefab prints 'message', 'default': '1'},
                          {'name': 'message', 'type': 'string', 'help': 'message to display n times', 'default': 'Hello World!'}]

            def __init__(self, *a, *k):
                Sofa.Core.Prefab.__init__(self, *a, **k)

            def doReInit(self):
                for i in range(0, self.n.value):
                    print(self.message.value)

        n = Sofa.Core.Node()
        n.addChild(Foo(name="aFooPrefab", n=42, message="hello universe!"))
    """
    def __init__(self, *args, **kwargs):
        Sofa.Core.RawPrefab.__init__(self, *args, **kwargs)
        frame = inspect.currentframe().f_back
        frameinfo = inspect.getframeinfo(frame)
        definedloc = (frameinfo.filename, frameinfo.lineno)

        self.setDefinitionSourceFileName(definedloc[0])
        self.setDefinitionSourceFilePos(definedloc[1])
        self.setSourceTracking(definedloc[0])

        frame = frame.f_back
        if frame is not None:
            frameinfo = inspect.getframeinfo(frame)
            definedloc = (frameinfo.filename, frameinfo.lineno)
            self.setInstanciationSourceFileName(definedloc[0])
            self.setInstanciationSourceFilePos(definedloc[1])

        self.setName(str(self.__class__.__name__))
        self.addData("prefabname", value=type(self).__name__, type="string", group="Infos", help="Name of the prefab")

        # A prefab should be added to its parent explicitely by calling parent.addChild() with this prefab.
        # However if for some reason you need to pass a context to your prefab, use the "parents" keyword argument to pass its context
        if "parent" in kwargs and "parents" not in kwargs:
            if isinstance(kwargs['parent'], Sofa.Core.Node):
                kwargs['parent'].addChild(self)
            else:
                Sofa.Helper.msg_error(self, "'parent' is a protected keyword on Sofa.Core.Prefab and should only be used to provide the Prefab's context node - if needed for its instantation")
        elif "parents" in kwargs and "parent" not in kwargs:
            if isinstance(kwargs["parents"], list):
                for parent in kwargs['parents']:
                    if isinstance(parent, Sofa.Core.Node):
                        parent.addChild(self)
                    else:
                        Sofa.Helper.msg_error(self, "'parents' is an optional protected keyword on Sofa.Core.Prefab and must contain the list of parents of the prefab - if needed for its instantiation")
            else:
                Sofa.Helper.msg_error(self, "'parents' is a protected keyword on Sofa.Core.Prefab and should contain the list of parents for the prefab - if needed for its instantiation.")
        else:
            if "parent" in kwargs and "parents" in kwargs:
                Sofa.Helper.msg_error(self, "Cannot use both 'parent' and 'parents' keywords on a prefab. Use 'parent' to set the context of your prefab, 'parents' in the case of a multi-parent prefab")
        
        # Prefab parameters are defined in a list of dictionaries named "properties".
        # The dictionaries has 3 required fields (name, type, help) and an additional optional field "default"
        docstring = ""
        if hasattr(self, "properties"):
            docstring += ""
            for p in self.properties:
                self.addPrefabParameter(name=p['name'], type=p['type'], help=p['help'], default=kwargs.get(p['name'], p.get('default', None)))
                docstring += "\n:param " + p['name'] + ": " + p['help'] + ", defaults to " + str(p.get('default', '')) + '\n:type ' + p['name'] + ": " + p['type'] + "\n\n"

        self.addData("docstring", value=('' if self.__doc__ is None else self.__doc__) + docstring, type="string", group="Infos", help="Documentation of the prefab")
        self.init()
