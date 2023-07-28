import Sofa
from Sofa.Core import TaskScheduler
import time

def createScene(root):
    root.dt = 0.01

    root.addObject("RequiredPlugin", pluginName=[
        "MultiThreading",
        "Sofa.Component.Constraint.Projective",
        "Sofa.Component.Engine.Select",
        "Sofa.Component.LinearSolver.Iterative",
        "Sofa.Component.Mass",
        "Sofa.Component.ODESolver.Backward",
        "Sofa.Component.StateContainer",
        "Sofa.Component.Topology.Container.Grid",
        "Sofa.Component.Visual",
        "Sofa.Component.Topology.Container.Dynamic",
    ])
    root.addObject('VisualStyle', displayFlags="showForceFields")
    root.addObject('DefaultAnimationLoop')
    root.addObject('EulerImplicitSolver', rayleighStiffness=0.1, rayleighMass=0.1)
    root.addObject('CGLinearSolver', iterations=25, tolerance=1.0e-9, threshold=1.0e-9)
    root.addObject('MechanicalObject', template="Vec3")
    root.addObject('UniformMass', vertexMass=1)
    root.addObject('RegularGridTopology', nx=8, ny=8, nz=40, xmin=-1.5, xmax=1.5, ymin=-1.5, ymax=1.5, zmin=0, zmax=19)
    root.addObject('BoxROI', box=[-1.5, -1.5, 0, 1.5, 1.5, 0.0001], name="box")
    root.addObject('FixedConstraint', indices="@box.indices")
    root.addObject('ParallelHexahedronFEMForceField', youngModulus=400000, poissonRatio=0.4, method="large", updateStiffnessMatrix=False)


# When not using runSofa, this main function will be called python
def main():
    root = Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.init(root)

    for nb_threads in range(1, TaskScheduler.GetHardwareThreadsCount() + 1):
        print(f'Number of threads #{nb_threads}')
        Sofa.Simulation.reset(root)
        TaskScheduler.init(nb_threads)

        start = time.time()
        for iteration in range(500):
            Sofa.Simulation.animate(root, root.dt.value)
        end = time.time()
        print(end - start, "seconds")

    print("Stopping...")
    TaskScheduler.stop()
    print("Stopped")



if __name__ == '__main__':
    main()
