/*********************************************************************
Copyright 2019, CNRS, University of Lille, INRIA

This file is part of sofaPython3

sofaPython3 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

sofaPython3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/
/********************************************************************
 Contributors:
    - damien.marchal@univ-lille.fr
    - bruno.josue.marques@inria.fr
    - eve.le-guillou@centrale.centralelille.fr
    - jean-nicolas.brunet@inria.fr
    - thierry.gaugry@inria.fr
********************************************************************/

#pragma once

namespace sofapython3::doc::controller
{
static auto Controller = R"(
        An Interface for user interaction on SOFA Components.

        It can catch events to trigger actions, such as onAnimateBeginEvent, onAnimateEndEvent or onPythonScriptEvent.
        A new custom controller class needs to be defined to use a controller in a script,
        and that class needs to reimplement the __init__ method.

        :example of use:

        In the following example, we redefine the controller class, and reimplement the __init__ method.
        We also implement the onAnimateBeginEvent, that will be activted everytime an animation step ends
        and that will simply print a message in the command line. In the createScene function, we initialize
        the controller and add it to the rootNode.
        If you run this with runSofa, it will simply endlessly print `onAnimateBeginEvent`
        when you click the Animate button.

            .. code-block:: python

                import Sofa.Core

                class MyController(Sofa.Core.Controller):
                    def __init__(self, *args, **kwargs):
                         ## These are needed (and the normal way to override from a python class)
                         Sofa.Core.Controller.__init__(self, *args, **kwargs)
                         print(" Python::__init__::"+str(self.name))

                    def onEvent(self, event):
                         """This function is the fallback one that is called if the XXXX event is
                            received but there is not overriden onXXXX() method.
                         """
                         print("generic event handler catched ", event)

                    def onAnimateBeginEvent(self, event):
                         print("onAnimateBeginEvent")

                def createScene(rootNode):
                    controller = MyController(name="MyC")
                    rootNode.addObject(controller)
                    return rootNode
         )";
}
