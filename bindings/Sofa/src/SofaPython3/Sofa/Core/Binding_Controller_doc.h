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
                    def onAnimateBeginEvent(self, event):
                         print("onAnimateBeginEvent")

                def createScene(rootNode):
                    controller = MyController(name="MyC")
                    rootNode.addObject(controller)
                    return rootNode

         )";
}
