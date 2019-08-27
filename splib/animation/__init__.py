# -*- coding: utf-8 -*-
"""
Animation framework focusing in ease of use.

**********
Functions:
**********

.. autosummary::

    AnimationManager
    AnimationManagerController
    addAnimation
    realTimeAnimation


splib.animation.addAnimation
***********************
.. autofunction:: addAnimation

splib.animation.AnimationManager
********************************
.. autofunction:: AnimationManager

splib.animation.realTimeAnimation
********************************
.. autofunction:: realTimeAnimation

splib.animation.AnimationManagerController
******************************************
.. autoclass:: AnimationManagerController(Sofa.Core.Controller)
   :members: addAnimation


********
Modules:
********

.. autosummary::
    :toctree: _autosummary

    splib.animation.easing

"""
__all__=["animate", "easing"]

from .animate import AnimationManager, AnimationManagerController, addAnimation, realTimeAnimation
