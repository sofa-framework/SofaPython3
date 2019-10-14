import Sofa

class MyController(Sofa.Core.Controller):
    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, *kwargs)
                
    def onEvent(self, params):
        print("Un-handled handled event received "+str(params))     

    def onSimulationInitDoneEvent(self, params):
        print("Handled event received: " + str(params))    

def createScene(node):
        node.addObject( MyController() )
        return node
