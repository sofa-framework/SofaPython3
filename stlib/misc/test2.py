import Sofa.Core
import copy
import entity
from entity import PrefabParameters, EntityParameters, Entity, Simulation


oldAdd=Sofa.Core.Node.addObject
def myAddObject(self : Sofa.Core.Node, tname, **kwargs):
    kwargs = copy.copy(kwargs)
    previouslen = len(self.objects) 
    try:
        oldAdd(self, tname, **kwargs)
    except Exception as e:
        target = self
        if len(self.objects) != previouslen:
            target = list(self.objects)[-1]
        Sofa.msg_error(target, str(e))
        
Sofa.Core.Node.addObject = myAddObject


def myAdd(self : Sofa.Core.Node, c, parameters = PrefabParameters(), **kwargs):  
    def findName(cname, node):
        """Compute a working unique name in the node"""
        rname = cname 
        for i in range(0, len(node.children)):
            if rname not in node.children:
                return rname
            rname = cname + str(i+1)
        return rname

    for k,v in kwargs.items():
        if hasattr(parameters, k):
            setattr(parameters, k, v)

    parameters = copy.copy(parameters)
    if parameters.name in self.children:
        parameters.name = findName(parameters.name, self)

    return c(parent = self, parameters=parameters) 
Sofa.Core.Node.add = myAdd

def createScene(root):
    #@optionalkwargs

    #def eulalieAddOde(self, **kwargs):
    #    self.addObject("EulerExplicitSolver", name="numericalintegration")
    #    self.addObject("LinearSolver", name="numericalsolver", firstOrder=True) 

    parameters = EntityParameters()
    parameters.simulation.iterations = 10
    parameters.simulation.integration["rayleighStiffness"] = 2.0
    parameters.addSimulation = entity.NONE
    
    parameters.mechanical["template"] = "Rigid3"

    #parameters.simulation.integration["rayleightStiffnessXXX"] = 2.0

    #parameters.solver.kwargs["numericalintegration"] = { "firstOrder" : True }

    root.add(Entity, parameters)
    root.add(Entity, parameters)
    root.add(Entity, parameters)

    #root.add(Simulation, name="mySimulation")
