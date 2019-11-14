# -*- coding: utf-8 -*-
#/******************************************************************************
#*       SOFA, Simulation Open-Framework Architecture, development version     *
#*                (c) 2006-2015 INRIA, USTL, UJF, CNRS, MGH                    *
#*                                                                             *
#* This library is free software; you can redistribute it and/or modify it     *
#* under the terms of the GNU Lesser General Public License as published by    *
#* the Free Software Foundation; either version 2.1 of the License, or (at     *
#* your option) any later version.                                             *
#*                                                                             *
#* This library is distributed in the hope that it will be useful, but WITHOUT *
#* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
#* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
#* for more details.                                                           *
#*                                                                             *
#* You should have received a copy of the GNU Lesser General Public License    *
#* along with this library; if not, write to the Free Software Foundation,     *
#* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
#*******************************************************************************
#*                              SOFA :: Framework                              *
#*                                                                             *
#* Authors: damien.marchal@univ-lille1.fr Copyright (C) CNRS                   *
#*                                                                             *
#* Contact information: contact@sofa-framework.org                             *
#******************************************************************************/

import traceback
import imp
import types
import gc
import os

def mimport(modulename, filename):
    f = open(filename, 'r')
    return imp.load_module(modulename, f, filename, (modulename, 'r', imp.PY_SOURCE))

def mreload(modulepath):
        try:
           #print("============== Updating a module ==========================")
           #print("LiveCoding: updating module from: "+modulepath)
           m=mimport(modulepath, modulepath)

           for obj in gc.get_objects():
              try:
                      if isinstance(obj, object) and obj.__class__.__name__ == "InnerSofaPrefab":
                                        print("OBJECT GC LEVEL B : " + str(obj))
                                        if hasattr(obj, "cb"):
                                                print("MATCH AND PATCH: ", obj.cb)
                                                if hasattr(m, str(obj.cb.__name__)  ):
                                                        print("GOT REPLACEMENT...", obj.cb)
                                                        print("GOT REPLACEMENT...", getattr(m, str(obj.cb.__name__)).__original__)
                                                        setattr(obj, "cb", getattr(m, str(obj.cb.__name__)))
                      if isinstance(obj, object) and hasattr(obj, "__class__"):
                        if hasattr(m, str(obj.__class__.__name__) ):
                            print("OBJECT GC LEVEL A : " + str(obj.__class__.__name__))
                            if hasattr(obj, "__module__"):
                                print("OBJECT GC LEVEL 0 : " + str(obj.__class__.__name__))
                                print("OBJECT GC LEVEL 0 : ", str(obj.__module__), str(m.__file__))

                                if str(obj.__module__) in ["__main__", os.path.basename(str(m.__file__))]:
                                    print("METHOD IS A : ", getattr(obj, "doReInit"))

                                    #obj.__class__=getattr(m, str(obj.__class__.__name__))
                                    print("METHOD IS B: ", getattr(obj, "doReInit"))

                                    mm = getattr(getattr(m, str(obj.__class__.__name__)), "doReInit")
                                    mm = mm.__get__(obj, obj.__class__)
                                    print("METHOD IS X: ",  getattr(obj, "doReInit"))
                                    setattr(obj, "doReInit", mm)
                                    print("METHOD IS 2: ",  getattr(obj, "doReInit"))

                                    print(obj.__class__)

                                    print(getattr(m, str(obj.__class__.__name__)))
                                    print("OBJECT GC: " + str(obj))

                                    if hasattr(obj, "onRecompile"):
                                            obj.onRecompile()
                                    continue

                      if isinstance(obj, types.ModuleType):
                        if not hasattr(m, "__file__" or m.__file__ == None):
                                continue;
                        if not hasattr(obj, "__file__") or obj.__file__ == None:
                                continue;

                        if obj.__file__.rstrip('c') == m.__file__:
                                for f in dir(obj):
                                        if isinstance(getattr(obj,f), types.FunctionType):
                                                print("Patching function: "+f+ " in " + str(type(obj)))
                                                setattr(obj, f, getattr(m, f))

              except:
                       traceback.print_exc()
           #print("================ End update ======================================")
        except:
           traceback.print_exc()

def onReimpAFile(filename):
        mreload(filename)


