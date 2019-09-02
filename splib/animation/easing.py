# -*- coding: utf-8 -*-
"""
Easing function to use in animation
"""

import math

def LinearRamp(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    """Linear interpolation between two values
    Examples:
        LinearRamp(10, 20, 0.5)
    """
    return (endValue-beginValue) * currentTime/totalTime + beginValue

"""
Sine easing functions
"""
def sineIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return -change * math.cos(currentTime/totalTime * (math.pi/2)) + change + beginValue

def sineOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * math.sin(currentTime/totalTime * (math.pi/2)) + beginValue

def sineInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return -change/2*(math.cos(math.pi*currentTime/totalTime)-1)+ beginValue

"""
Quadratic easing functions
"""
def quadIn(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * (currentTime / totalTime) **2 + beginValue

def quadOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return - change * (currentTime / totalTime) * (currentTime / totalTime - 2) + beginValue

def quadInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
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
    change = endValue - beginValue
    return change * (currentTime / totalTime)**3 + beginValue

def cubicOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * ((currentTime / totalTime - 1)**3 + 1)+ beginValue

def cubicInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
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
    change = endValue - beginValue
    return change * (currentTime / totalTime)**4 + beginValue

def quarticOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return - change * ((currentTime / totalTime - 1)**4 - 1)+ beginValue

def quarticInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
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
    change = endValue - beginValue
    return change * (currentTime / totalTime)**5 + beginValue

def quinticOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * ((currentTime / totalTime - 1)**5 + 1)+ beginValue

def quinticInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
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
    change = endValue - beginValue
    return change * math.pow(2,10 * (currentTime/totalTime - 1)) + beginValue

def expoOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * (-math.pow(2,-10 * currentTime/totalTime) + 1 ) + beginValue

def expoInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
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
    change = endValue - beginValue
    return - change * (math.sqrt(1 - (currentTime/totalTime)**2) - 1) + beginValue

def circOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    return change * (math.sqrt(1 - (currentTime/totalTime-1)**2)) + beginValue

def circInOut(currentTime, totalTime=1.0, beginValue=0.0, endValue=1.0):
    change = endValue - beginValue
    t = currentTime
    t /= totalTime/2
    if (t < 1):
        return - change / 2 * (math.sqrt( 1 - t**2) - 1) + beginValue
    t -= 2
    return change/2 * (math.sqrt(1 - t**2)+1) + beginValue
