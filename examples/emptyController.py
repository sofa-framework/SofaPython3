import Sofa


# This python script shows the functions to be implemented
# in order to create your Controller in python
class EmptyController(Sofa.Core.Controller):

    def __init__(self, *args, **kwargs):
        # These are needed (and the normal way to override from a python class)
        Sofa.Core.Controller.__init__(self, *args, **kwargs)

    # Default BaseObject functions********************************
    def init(self):
        pass

    def bwdInit():
        pass
    
    def reinit():
        pass
    
    # Default Events *********************************************
    def onAnimateBeginEvent(self, event): # called at each begin of animation step
        pass

    def onAnimateEndEvent(self, event): # called at each end of animation step
        pass

    def onKeypressedEvent(self, event):
        key = event['key']
        if ord(key) == 19:  # up
            print("You pressed the Up key")

        if ord(key) == 21:  # down
            print("You pressed the Down key")

        if ord(key) == 18:  # left
            print("You pressed the Left key")

        if ord(key) == 20:  # right
            print("You pressed the Right key")

    def onKeyreleasedEvent(self, event):
        key = event['key']
        if ord(key) == 19:  # up
            print("You released the Up key")

        if ord(key) == 21:  # down
            print("You released the Down key")

        if ord(key) == 18:  # left
            print("You released the Left key")

        if ord(key) == 20:  # right
            print("You released the Right key")

    def onMouseEvent(self, event):
        if (event['State']== 0): # mouse moving
            print("Mouse is moving (x,y) = "+str(event['mouseX'])+" , "+str(event['mouseY']))

        if (event['State']==1): # left mouse clicked
            print("Left mouse clicked")

        if (event['State']==2): # left mouse released
            print("Left mouse released")

        if (event['State']==3): # right mouse released
            print("Right mouse clicked")

        if (event['State']==4): # right mouse released
            print("Right mouse released")

        if (event['State']==5): # wheel clicked
            print("Mouse wheel clicked")

        if (event['State']==6): # wheel released
            print("Mouse wheel released")

    def onScriptEvent(self, event):
        pass

    def onEvent(self, event):
        pass


def createScene(root):
    root.dt = 0.01
    root.bbox = [[-1, -1, -1],[1,1,1]]
    root.addObject('DefaultVisualManagerLoop')
    root.addObject('DefaultAnimationLoop')

    # Add our python controller in the scene
    root.addObject( EmptyController(name="MyEmptyController") )


def main():
    import SofaRuntime
    import Sofa.Gui

    root=Sofa.Core.Node("root")
    createScene(root)

    Sofa.Gui.GUIManager.Init("myscene", "qglviewer")
    Sofa.Gui.GUIManager.createGUI(root, __file__)
    Sofa.Gui.GUIManager.SetDimension(1080, 1080)
    Sofa.Gui.GUIManager.MainLoop(root)
    Sofa.Gui.GUIManager.closeGUI()

    print("End of simulation.")


if __name__ == '__main__':
    main()
