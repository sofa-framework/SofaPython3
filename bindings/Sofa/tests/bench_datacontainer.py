# coding: utf8
import sys
import os
import numpy
import timeit
import Sofa.Core
import SofaRuntime

rawcpy = numpy.zeros((1000000,3), dtype=numpy.float64)

root = Sofa.Core.Node("root")
root.addObject("RequiredPlugin", name="SofaBaseMechanics")
obj = root.createObject("MechanicalObject", name="test", position=rawcpy.tolist())

it=10

def sofa1():
        p = obj.position.tolist()
        for i in range(len(p)):
                p[i] = 0.0
        obj.position = p

def f(i,j,v):
        return v*v

def bloc():
        with obj.position.writeable() as w:
                w += w
                w *= w
                w += 1.0
                w -= 2.0
                w *= w 

code = [
        #"sofa1()",
        "bloc()",
        "obj.position",
        "obj.position[0]=1.0",
        "obj.position[:]=1.0",
        "obj.position[:,:]=1.0",
        "obj.position[:,0]=1.0",
        "obj.position+=1.0",
        "obj.position-=1.0",
        "obj.position*=1.0",
        #"obj.position/=1.0",
        "obj.position.apply(f)"]          # Comment accélérer. En délégant à du code c.
                
for c in code:
        print(c, ": ", timeit.timeit(c, number=it, globals=globals()))

