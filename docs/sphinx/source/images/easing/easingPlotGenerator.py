import numpy as np
from splib.animation import easing
import matplotlib.pyplot as plt

"""
Python script to generate the images of the different easing functions in the Sphinx documentation
"""

Nbpoints = 150 

functionList = [
	easing.sineIn, easing.sineOut, easing.sineInOut,
	easing.quadIn, easing.quadOut, easing.quadInOut,
	easing.cubicIn, easing.cubicOut, easing.cubicInOut,
	easing.quarticIn, easing.quarticOut, easing.quarticInOut,
	easing.quinticIn, easing.quinticOut, easing.quinticInOut,
	easing.expoIn, easing.expoOut, easing.expoInOut,
	easing.circIn, easing.circOut, easing.circInOut,
	easing.LinearRamp
]

functionName = [
	"sineIn", "sineOut", "sineInOut",
	"quadIn", "quadOut","quadInOut",
	"cubicIn", "cubicOut", "cubicInOut",
	"quarticIn", "quarticOut", "quarticInOut",
	"quinticIn", "quinticOut", "quinticInOut",
	"expoIn", "expoOut", "expoInOut",
	"circIn", "circOut","circInOut",
	"LinearRamp"
]

color = [ "red", "green", "blue", "yellow", "orange", "purple", "brown", "grey"]

functionNameInOut = ["sineIn", "sineOut", "sineInOut"]

X = np.linspace(0, 1, Nbpoints ,endpoint=True)
for i in range(22):
	Y =[]
	for j in range(Nbpoints):
		Y.append(functionList[i](X[j]))
	k = i//3
	plt.plot(X, Y, label= functionName[i], color=color[k])
	plt.legend(loc='upper left')
	plt.savefig(functionName[i]+'.png')


