"""
Animation module
----------------
Animation framework focusing in ease of use.

Submodules:
  .. autosummary::
    :toctree: _autosummary

    splib.animation.animate
    splib.animation.easing




"""
__all__=["animate", "easing"]

from .animate import AnimationManager, AnimationManagerController, addAnimation
