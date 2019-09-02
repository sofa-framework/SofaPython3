import numpy as np
from splib.animation import easing
import matplotlib.pyplot as plt

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
	"sine",
	"quad",
	"cubic",
	"quartic",
	"quintic",
	"expo",
	"circ",
	"LinearRamp"
]

color = [ "red", "green", "blue", "yellow", "orange", "purple", "brown", "grey"]

functionNameInOut = ["sineIn", "sineOut", "sineInOut"]

X = np.linspace(0, 1, Nbpoints ,endpoint=True)
fig = plt.figure(figsize=(18,6))
fig.canvas.set_window_title("Easing functions")

plt.subplot(1, 2, 1)
for i in range(3):
	Y =[]
	for j in range(Nbpoints):
		Y.append(functionList[i](X[j]))
	plt.plot(X, Y, label=functionNameInOut[i])
plt.legend(loc='upper left')
plt.title("Comparing In, Out and InAndOut for the sinus easing functions")

plt.subplot(1, 2, 2)
for i in range(22):
	Y =[]
	for j in range(Nbpoints):
		Y.append(functionList[i](X[j]))
	k = i//3
	if (i/3 == k):
		plt.plot(X, Y, label=functionName[k], color=color[k])
	else:
		plt.plot(X, Y, color=color[k])
plt.legend(loc='upper left')
plt.title("Comparing all of the easing functions available in SofaPython3")
plt.show()


