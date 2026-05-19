/******************************************************************************
*                              SofaPython3 plugin                             *
*                  (c) 2021 CNRS, University of Lille, INRIA                  *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#pragma once

namespace sofapython3::doc::component
{
static auto componentClass =
        R"(
        Overridable class for SOFA Components

        It can catch events to trigger actions, such as onAnimateBeginEvent, onAnimateEndEvent or onPythonScriptEvent.
        A new custom Components class needs to be defined to use a Components in a script,
        and that class needs to reimplement the __init__ method.

        :example of use:

        In the following example, we redefine the Components class, and reimplement the __init__ method.
        We also implement the init and onAnimateBeginEvent methods, the last one will be activted everytime an animation step ends
        and that will simply print a message in the command line. In the createScene function, we initialize
        the Components and add it to the rootNode.
        If you run this with runSofa, it will first print "Init component" before you start the simulation, then
        it will simply endlessly print `onAnimateBeginEvent`

        when you click the Animate button.

            .. code-block:: python

                import Sofa.Core

                class MyComponent(Sofa.Core.Component):
                    def __init__(self, *args, **kwargs):
                        ## These are needed (and the normal way to override from a python class)
                        Sofa.Core.Component.__init__(self, *args, **kwargs)
                        print(" Python::__init__::"+str(self.name))

                    def init():
                        """This function is called at the initialization stage of the component.
                           Right after the call to 'createScene' when the root node is initialized.
                        """
                        print("Init component")

                    def onEvent(self, event):
                        """This function is the fallback one that is called if the XXXX event is
                           received but there is not overriden onXXXX() method.
                        """
                        print("generic event handler catched ", event)

                    def onAnimateBeginEvent(self, event):
                        print("onAnimateBeginEvent")

                def createScene(rootNode):
                    controller = MyComponent(name="MyC")
                    rootNode.addObject(controller)
                    return rootNode
         )";
}
