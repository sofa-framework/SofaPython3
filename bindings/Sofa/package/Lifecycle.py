"""
Activate/deactive some feature of sofa python

Use that to control how some part of the binding should behave.

Usage:
    from Sofa.Lifecycle import __new_feature__

    with __new_feature__("feature_name", True):
        do_something()

    with __new_feature__("feature_name", False):
            do_something()
"""
import Sofa.__Lifecycle__
from contextlib import ContextDecorator

### Initialize the feature set.
# Add your own feature by un-commenting the following line
Sofa.__Lifecycle__.init("my_feature", False)

def register_feature(feature_name, default_state):
    Sofa.__Lifecycle__.init(feature_name, default_state)

def get_feature_status(name):
    return Sofa.__Lifecycle__.get(name)

def list_features():
    return Sofa.__Lifecycle__.list_features()

class __new_feature__(ContextDecorator):
        def __init__(self, name, value):
            self.name=name
            self.new_value=value
            self.old_value=None

        def __enter__(self):
            self.old_value=Sofa.__Lifecycle__.get(self.name)
            Sofa.__Lifecycle__.set(self.name, self.new_value)
            return self

        def __exit__(self, *exc):
            Sofa.__Lifecycle__.set(self.name, self.old_value)
            return False
