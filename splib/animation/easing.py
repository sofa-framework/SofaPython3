# -*- coding: utf-8 -*-
"""
Easing function to use in animation

.. autosummary::

   LinearRamp

splib.animation.easing.LinearRamp
*********************************
.. autofunction:: LinearRamp

"""

import math

def LinearRamp(beginValue=0.0, endValue=1.0, scale=0.5):
    """Linear interpolation between two values

    Examples:
        LinearRamp(10, 20, 0.5)

    """
    return (endValue-beginValue) * scale + beginValue

"""
Sine easing functions
"""
def SineIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return -change * math.cos(currentTime/totalTime * (math.pi/2)) + change + beginValue

def SineOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * math.sin(currentTime/totalTime * (math.pi/2)) + beginValue

def SineInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return -change/2*(math.cos(math.pi*currentTime/totalTime)-1)+ beginValue

"""
Quadratic easing functions
"""
def QuadIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * (currentTime / totalTime) **2 + beginValue

def QuadOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return - change * (currentTime / totalTime) * (currentTime / totalTime - 2) + beginValue

def QuadInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    if ( 2 * currentTime /totalTime < 1):
        return (change/2)*(2 * currentTime / totalTime)**2 + beginValue
    return -change / 2 * (2 * currentTime / totalTime - 1)**2 + beginValue

"""
Cubic easing functions
"""

def CubicIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * (currentTime / totalTime)**3 + beginValue

def CubicOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * ((currentTime / totalTime - 1)**3 + 1)+ beginValue

def CubicInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    if (2 * currentTime / totalTime < 1):
        return change / 2 * (currentTime / totalTime)**3 + beginValue
    return change / 2 * ((currentTime / totalTime - 2)**3 + 2)+ beginValue

"""
Quartic easing functions
"""

def QuarticIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * (currentTime / totalTime)**4 + beginValue

def QuarticOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return - change * ((currentTime / totalTime - 1)**4 - 1)+ beginValue

def QuarticInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    if (2 * currentTime / totalTime < 1):
        return change / 2 * (currentTime / totalTime)**4 + beginValue
    return - change / 2 * ((currentTime / totalTime - 2)**4 - 2)+ beginValue

"""
Quintic easing functions
"""

def QuinticIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * (currentTime / totalTime)**5 + beginValue

def QuinticOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * ((currentTime / totalTime - 1)**5 + 1)+ beginValue

def QuinticInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    if (2 * currentTime / totalTime < 1):
        return change / 2 * (currentTime / totalTime)**5 + beginValue
    return change / 2 * ((currentTime / totalTime - 2)**5 + 2)+ beginValue

"""
Exponential easing functions
"""

def ExpoIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * (2**(10 * (currentTime/totalTime - 1))) + beginValue

def ExpoOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * (-2**(-10 * currentTime/totalTime) + 1 ) + beginValue

def ExpoInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    if (2*t/d < 1):
        return change/2 * 2**(10*(2*currentTime/totalTime - 1)) + beginValue
    return change/2 * (- 2**(-10 * (2*currentTime/totalTime - 1))) + beginValue

"""
Circular easing functions
"""

def CircIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return - change * (math.sqrt(1 - (currentTime/totalTime)**2) - 1) + beginValue

def CircOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * (math.sqrt(1 - (currentTime/totalTime-1)**2)) + beginValue

def CircInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    if (2*currentTime/totalTime <1):
        - change / 2 * (math.sqrt( 1 - (2*currentTime/totalTime)**2) - 1) + beginValue
    return change/2 * (math.sqrt(1 - (2*currentTime/totalTime-2)**2)+1) + beginValue

