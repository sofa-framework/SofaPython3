"""
Control which feature of the binding are activated
--------------------------------------------------
"""
import Sofa.Config
import sys

class Foo:
    def __init__(self):
        Sofa.Config.init("object_auto_init", False)

    def __setattr__(self, name, value):
        Sofa.Config.set(name, value)

    def __getattr__(self, name):
        return Sofa.Config.get(name)

    def __dir__(self):
        return Sofa.Config.list_features()

# Hack from Ruido von Rossum.
sys.modules[__name__] = Foo()
