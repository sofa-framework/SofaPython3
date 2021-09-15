"""
Activate/deactive some feature of sofa python

Use that to control how some part of the binding should behave.

Usage:
    from Sofa.future import __enable_feature__

    with __enable_feature__("feature_name", True):
        do_something()

    with __enable_feature__("feature_name", False):
            do_something()
"""
import Sofa.Config
from contextlib import ContextDecorator

### Initialize the feature set.
# Add your own feature by un-commenting the following line
#Sofa.Config.init("my_feature", False)
Sofa.Config.init("object_auto_init", False)

def has_feature(name):
    return Sofa.Config.get(name)

def list_features():
    return Sofa.Config.list_features()

class __enable_feature__(ContextDecorator):
        @staticmethod
        def list_features():
            return self.Config.list_feature()

        def __init__(self, name, value):
            self.name=name
            self.new_value=value
            self.old_value=None

        def __enter__(self):
            self.old_value=Sofa.Config.get(self.name)
            Sofa.Config.set(self.name, self.new_value)
            return self

        def __exit__(self, *exc):
            Sofa.Config.set(self.name, self.old_value)
            return False
