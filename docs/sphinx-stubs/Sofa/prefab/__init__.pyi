import Sofa.prefab
import typing
import Sofa
import Sofa.Core
import inspect

__all__ = [
    "Prefab",
    "Sofa",
    "inspect"
]


class Prefab(Sofa.Core.RawPrefab, Sofa.Core.BasePrefab, Sofa.Core.Node, Sofa.Core.BaseNode, Sofa.Core.Context, Sofa.Core.BaseContext, Sofa.Core.Base):
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
    pass
