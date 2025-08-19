import multiprocessing
import threading
import time
import rpyc
from SOFAService import SOFAService, SOFAClient
import Sofa



def createScene(root):
    #Need to use getValue and setValue instead of the syntax `root.dt = 0.02` or `root.dt.value` to get the actual value
    root.gravity.setValue([0, -9.81, 0])
    root.dt.setValue(0.02)

    #RPyC deal in a strange way with list of strings, which leads to error if you use the syntax where you pass a list of plugin name to required plugin. You need to add one required plugin per plugin for it to work, such as done in xml.
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.Collision.Detection.Algorithm')
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.Collision.Detection.Intersection')
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.Collision.Geometry')
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.Collision.Response.Contact')
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.Constraint.Projective')
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.IO.Mesh')
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.LinearSolver.Iterative')
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.Mapping.Linear')
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.Mass')
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.ODESolver.Backward')
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.SolidMechanics.FEM.Elastic')
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.StateContainer')
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.Topology.Container.Dynamic')
    root.addObject("RequiredPlugin", pluginName='Sofa.Component.Visual')
    root.addObject("RequiredPlugin", pluginName='Sofa.GL.Component.Rendering3D')

    root.addObject('DefaultAnimationLoop')

    root.addObject('VisualStyle', displayFlags="showCollisionModels")
    root.addObject('CollisionPipeline', name="CollisionPipeline")
    root.addObject('BruteForceBroadPhase', name="BroadPhase")
    root.addObject('BVHNarrowPhase', name="NarrowPhase")
    root.addObject('CollisionResponse', name="CollisionResponse", response="PenalityContactForceField")
    root.addObject('DiscreteIntersection')

    #Don't forget that this will be launched in the remote, all files should be on its disk or else, use lambda function to capture data and access theme in this funciton
    root.addObject('MeshOBJLoader', name="LiverSurface", filename="mesh/liver-smooth.obj")

    liver = root.addChild('Liver')
    liver.addObject('EulerImplicitSolver', name="cg_odesolver", rayleighStiffness="0.1", rayleighMass="0.1")
    liver.addObject('CGLinearSolver', name="linear_solver", iterations="25", tolerance="1e-09", threshold="1e-09")
    liver.addObject('MeshGmshLoader', name="meshLoader", filename="mesh/liver.msh")
    liver.addObject('TetrahedronSetTopologyContainer', name="topo", src="@meshLoader")
    liver.addObject('MechanicalObject', name="dofs", src="@meshLoader")
    liver.addObject('TetrahedronSetGeometryAlgorithms', template="Vec3d", name="GeomAlgo")
    liver.addObject('DiagonalMass', name="Mass", massDensity="1.0")
    liver.addObject('TetrahedralCorotationalFEMForceField', template="Vec3d", name="FEM", method="large", poissonRatio="0.3", youngModulus="3000", computeGlobalMatrix="0")
    liver.addObject('FixedProjectiveConstraint', name="FixedConstraint", indices="3 39 64")

    visu = liver.addChild('Visu')
    visu.addObject('OglModel', name="VisualModel", src="@../../LiverSurface")
    visu.addObject('BarycentricMapping', name="VisualMapping", input="@../dofs", output="@VisualModel")

    surf = liver.addChild('Surf')
    surf.addObject('SphereLoader', name="sphereLoader", filename="mesh/liver.sph")
    surf.addObject('MechanicalObject', name="spheres", position="@sphereLoader.position")
    surf.addObject('SphereCollisionModel', name="CollisionModel", listRadius="@sphereLoader.listRadius")
    surf.addObject('BarycentricMapping', name="CollisionMapping", input="@../dofs", output="@spheres")

    return root



if __name__ == "__main__":


    SC = SOFAClient()
    SC.start_server(port=18818)
    SC.connect_client(port=18818)

    SC.load_scene(createScene)

    #This works only for server and client on the same machine. This tell the server that the passed data path should be copied in shared memory when accessed through the client instead of RPyC.
    SC.setup_shared_memory_for_data(["Liver/dofs.position","Liver/Surf/spheres.position"])



    asynch_step = None
    currentTime = 0.0
    while currentTime<0.2:
        if(asynch_step is None or asynch_step.ready):
            #Time to get data from object
            currentTime = SC.sofa_root.getTime()
            print(currentTime)
    
            print(f"This comes with the socket : {SC.sofa_root.Liver.cg_odesolver.name.value}")
            print(f"This comes with shared memory : {SC.sofa_root.Liver.Surf.spheres.position.value}")
            print(f"This getValue comes with the socket : {SC.sofa_root.Liver.cg_odesolver.name.getValue()}")
            print(f"This getValue comes with shared memory : {SC.sofa_root.Liver.Surf.spheres.position.getValue()}")
            SC.sofa_root.Liver.cg_odesolver.printLog.setValue(True)
            SC.sofa_root.Liver.Surf.spheres.position.setValue([[0,0,0]])

            #Launch next step
            asynch_step = SC.asynch_step()
        else:
            print("waiting 0.1 sec")
            time.sleep(0.1)

    SC.stop_server()
    
