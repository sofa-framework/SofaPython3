#! python3
import Sofa
from SofaRuntime import Timer


# This controller will extract the timer records from the simulation at each steps
class TimerController(Sofa.Core.Controller):

    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)

        # This is needed to avoid a conflict with the timer of runSofa
        self.use_sofa_profiler_timer = False

    def onAnimateBeginEvent(self, event):
        if len(Timer.getRecords('Animate')):
            self.use_sofa_profiler_timer = True
        else:
            Timer.setEnabled("cg_timer", True)
            Timer.begin("cg_timer")

    def onAnimateEndEvent(self, event):
        if self.use_sofa_profiler_timer:
            records = Timer.getRecords("Animate")
        else:
            records = Timer.getRecords("cg_timer")

        step_time = records['solve']['Mechanical (meca)']['total_time']
        print(f"Step took {step_time:.2f} ms")

        nb_iterations = records['solve']['Mechanical (meca)']['StaticSolver::Solve']['nb_iterations']
        for i in range(int(nb_iterations)):
            total_time = records['solve']['Mechanical (meca)']['StaticSolver::Solve']['NewtonStep'][i]['total_time']
            CG_iterations = records['solve']['Mechanical (meca)']['StaticSolver::Solve']['NewtonStep'][i]['MBKSolve']['CG iterations']
            print(f"  Newton iteration #{i} took {total_time:.2f} ms using {int(CG_iterations)} CG iterations")

        if not self.use_sofa_profiler_timer:
            Timer.end("cg_timer")


# Scene creation - This is automatically called by SofaPython3 when using runSofa
def createScene(root):
    root.dt = 0.01

    # List of required plugins
    root.addObject("RequiredPlugin", pluginName=['Sofa.Component.Constraint.Projective',
    'Sofa.Component.Engine.Select',
    'Sofa.Component.LinearSolver.Iterative',
    'Sofa.Component.MechanicalLoad',
    'Sofa.Component.ODESolver.Backward',
    'Sofa.Component.SolidMechanics.FEM.Elastic',
    'Sofa.Component.StateContainer',
    'Sofa.Component.Topology.Container.Dynamic',
    'Sofa.Component.Topology.Container.Grid',
    'Sofa.Component.Visual'
    ])


    # AnimationLoop
    root.addObject('DefaultAnimationLoop')

    # Visual style
    root.addObject('VisualStyle', displayFlags='showBehaviorModels showForceFields')

    # Add the python controller in the scene
    root.addObject( TimerController() )

    # Create a grid topology of 10x10x60 centered on (0,0,0)
    root.addObject('RegularGridTopology', name='grid', min=[-5, -5, -30], max=[5, 5, 30], n=[6, 6, 16])

    # Create our mechanical node
    root.addChild("meca")
    root.meca.addObject("StaticSolver", newton_iterations=5, printLog=False)
    root.meca.addObject("CGLinearSolver", iterations=25, tolerance=1e-5, threshold=1e-5)

    root.meca.addObject('MechanicalObject', name='mo', position='@../grid.position')
    root.meca.addObject('HexahedronSetTopologyContainer', name='mechanical_topology', src='@../grid')
    root.meca.addObject('HexahedronFEMForceField', youngModulus=3000, poissonRatio=0)

    root.meca.addObject('BoxROI', name='base_roi', box=[-5.01, -5.01, -30.01, 30.01, 30.01, -29.99])
    root.meca.addObject('BoxROI', name='top_roi',  box=[-5.01, -5.01, +29.99, 5.01, 5.01, +30.01], quad='@mechanical_topology.quads')

    root.meca.addObject('FixedConstraint', indices='@base_roi.indices')
    root.meca.addObject('QuadSetGeometryAlgorithms')
    root.meca.addObject('QuadPressureForceField', pressure=[0, -30, 0], quadList='@top_roi.quadInROI', showForces=False)


# When not using runSofa, this main function will be called python
def main():
    root = Sofa.Core.Node("root")
    createScene(root)
    Sofa.Simulation.init(root)

    # Run the simulation for 5 steps
    for iteration in range(5):
        print(f'Iteration #{iteration}')
        Sofa.Simulation.animate(root, root.dt.value)


if __name__ == '__main__':
    main()
