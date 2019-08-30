# -*- coding: utf-8 -*-
"""
Easing functions for smoother animations
----------------------------------------
Easing functions allow you to apply custom mathematical formulas to your animations. It helps making the animation smoother.
You can see below the different behaviors of the easing functions available in SofaPython3.

:Example of use:
    .. code-block:: python

        from splib.animation import easing

        easing.sineIn(0.4)
        easing.quarticInOut(1,10,0.2,10.3)

.. image:: ../../../../images/easing/easingScene.gif
        :alt: Different behavior of the easing functions available in SofaPython3
        :align: center
        :height: 250pt

:Easing function implemented in SofaPython3:

.. autofunction:: LinearRamp
.. autofunction:: sineIn
.. autofunction:: sineOut
.. autofunction:: sineInOut
.. autofunction:: quadIn
.. autofunction:: quadOut
.. autofunction:: quadInOut
.. autofunction:: cubicIn
.. autofunction:: cubicOut
.. autofunction:: cubicInOut
.. autofunction:: quarticIn
.. autofunction:: quarticOut
.. autofunction:: quarticInOut
.. autofunction:: quinticIn
.. autofunction:: quinticOut
.. autofunction:: quinticInOut
.. autofunction:: expoIn
.. autofunction:: expoOut
.. autofunction:: expoInOut
.. autofunction:: circIn
.. autofunction:: circOut
.. autofunction:: circInOut
"""

import math

def LinearRamp(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """Linear interpolation between two values

    .. image:: ../../../../images/easing/LinearRamp.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    return (endValue-beginValue) * currentTime/totalTime + beginValue

"""
Sine easing functions
"""
def sineIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/sineIn.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return -change * math.cos(currentTime/totalTime * (math.pi/2)) + change + beginValue

def sineOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/sineOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return change * math.sin(currentTime/totalTime * (math.pi/2)) + beginValue

def sineInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/sineInOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return -change/2*(math.cos(math.pi*currentTime/totalTime)-1)+ beginValue

"""
Quadratic easing functions
"""
def quadIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/quadIn.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return change * (currentTime / totalTime) **2 + beginValue

def quadOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/quadOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return - change * (currentTime / totalTime) * (currentTime / totalTime - 2) + beginValue

def quadInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/quadInOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    t = currentTime
    t /= totalTime/2
    if (t < 1):
        return (change/2)*t**2 + beginValue
    t -= 1
    return -change / 2 * (t*(t-2) - 1) + beginValue

"""
Cubic easing functions
"""

def cubicIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/cubicIn.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return change * (currentTime / totalTime)**3 + beginValue

def cubicOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/cubicOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return change * ((currentTime / totalTime - 1)**3 + 1)+ beginValue

def cubicInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/cubicInOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    t = currentTime
    t /= totalTime/2
    if (t < 1):
        return change / 2 * (t**3) + beginValue
    t -= 2
    return change / 2 * ((t**3) + 2)+ beginValue

"""
Quartic easing functions
"""

def quarticIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/quarticIn.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return change * (currentTime / totalTime)**4 + beginValue

def quarticOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/quarticOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return - change * ((currentTime / totalTime - 1)**4 - 1)+ beginValue

def quarticInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/quarticInOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    t = currentTime
    t /= totalTime/2
    if (t < 1):
        return change / 2 * (t**4) + beginValue
    t -= 2
    return - change / 2 * ((t**4) - 2)+ beginValue

"""
Quintic easing functions
"""

def quinticIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/quinticIn.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return change * (currentTime / totalTime)**5 + beginValue

def quinticOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/quinticOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return change * ((currentTime / totalTime - 1)**5 + 1)+ beginValue

def quinticInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/quinticInOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    t = currentTime
    t /= totalTime/2
    if (t < 1):
        return change / 2 * (t**5) + beginValue
    t -= 2
    return change / 2 * ((t**5) + 2)+ beginValue

"""
Exponential easing functions
"""

def expoIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/expoIn.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return change * math.pow(2,10 * (currentTime/totalTime - 1)) + beginValue

def expoOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/expoOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return change * (-math.pow(2,-10 * currentTime/totalTime) + 1 ) + beginValue

def expoInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/expoInOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    t = currentTime
    t /= totalTime/2
    if (t < 1):
        return change/2 * math.pow(2,10*(t - 1)) + beginValue
    t -= 1
    return change/2 * (-math.pow(2,-10 * t) + 2 ) + beginValue

"""
Circular easing functions
"""

def circIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/circIn.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return - change * (math.sqrt(1 - (currentTime/totalTime)**2) - 1) + beginValue

def circOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/circOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    return change * (math.sqrt(1 - (currentTime/totalTime-1)**2)) + beginValue

def circInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """
    .. image:: ../../../../images/easing/circInOut.png
        :alt: Behavior of the easing function
        :align: center
        :height: 200pt

    """
    change = endValue - beginValue
    t = currentTime
    t /= totalTime/2
    if (t < 1):
        return - change / 2 * (math.sqrt( 1 - t**2) - 1) + beginValue
    t -= 2
    return change/2 * (math.sqrt(1 - t**2)+1) + beginValue
