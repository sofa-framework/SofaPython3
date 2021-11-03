# coding: utf8

import sys
import os
import numpy
import timeit
sys.path.append("./Sofa/package")
sys.path.append("./SofaRuntime/package")

import Sofa.Core
import SofaRuntime

it=1000

root = Sofa.Core.Node("rootNode")
c = []
for i in range(0,1000):
        newchild = root.addChild("Child"+str(i))
        c.append(newchild)
        
parented = root.addChild("parented")
for child in root.children:
        child.addChild(parented)        

def newIt():
        c = []
        for e in root.children:
                c.append(e)         

def newRange():
        c = []
        for e in range(len(root.children)):
                c.append(root.children[e])         

def newRangeF():
        c = []
        d = root.children
        for e in range(len(d)):
                c.append(d[e])         



code = ["parented.parents",
        "parented.parents[0]",
        "list(parented.parents)",
        "len(parented.parents)",
        "root.children",
        "root.children[50]",
        "len(root.children)",
        "list(root.children)",
        "newIt()",
        "newRange()",
        "newRangeF()"]

for c in code:
        print(c, timeit.timeit(c, number=it, globals=globals()))

