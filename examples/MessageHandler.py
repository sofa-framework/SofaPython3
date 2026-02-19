import Sofa
import Sofa.Helper

RED = "\033[31m"
ORANGE = "\033[38;5;214m"
GREEN = "\033[32m"
RESET = "\033[0m"


class ExempleMessageHandler(Sofa.Helper.MessageHandler):
    """A custom message handler that prints messages in the console and counts the number of errors, warnings and infos."""

    def __init__(self):
        super().__init__()
        self.num_errors = 0
        self.num_warnings = 0
        self.num_infos = 0

    def process(self, msg):
        """Prints the message in the console and count the number of errors, warnings and infos."""
        if msg['type'] == "Error":
            self.print_error(msg['sender'], msg['message'])
            self.num_errors += 1
        elif msg['type'] == "Warning":
            self.print_warning(msg['sender'], msg['message'])
            self.num_warnings += 1
        elif msg['type'] == "Info":
            self.print_info(msg['sender'], msg['message'])
            self.num_infos += 1
        else:
            print(f"{msg['type']} {msg['message']}")

    @staticmethod
    def print_error(sender, message):
        """Prints a string with [ERROR] in red"""
        print(f"🚨{RED}[ERROR]{RESET} [👤{sender}] 📨{message}")

    @staticmethod
    def print_warning(sender, message):
        """Prints a string with [WARNING] in orange"""
        print(f"⚠️{ORANGE}[WARNING]{RESET} [👤{sender}] 📨{message}")

    @staticmethod
    def print_info(sender, message):
        """Prints a string with [INFO] in green"""
        print(f"ℹ️{GREEN}[INFO]{RESET} [👤{sender}] 📨{message}")


with ExempleMessageHandler() as msg_handler:
    def createScene(root):

        root.addObject("RequiredPlugin", pluginName=[
            'Sofa.Component.Constraint.Projective',
            'Sofa.Component.Engine.Select',
            'Sofa.Component.LinearSolver.Direct',
            'Sofa.Component.Mass',
            'Sofa.Component.ODESolver.Backward',
            'Sofa.Component.SolidMechanics.FEM.Elastic',
            'Sofa.Component.StateContainer',
            'Sofa.Component.Topology.Container.Grid',
            'Sofa.Component.Visual'
        ])

        root.addObject('VisualStyle', displayFlags="showBehaviorModels showForceFields")

        root.addObject('DefaultAnimationLoop')
        root.addObject('DefaultVisualManagerLoop')

        root.addObject('EulerImplicitSolver', rayleighStiffness=0.1, rayleighMass=0.1, printLog=False)
        root.addObject('SparseLDLSolver', template="CompressedRowSparseMatrixd")

        root.addObject('MechanicalObject', name="DoFs")
        root.addObject('MeshMatrixMass', name="mass", totalMass=320)
        root.addObject('RegularGridTopology', name="grid",
                       nx=4, ny=4, nz=20, xmin=-9, xmax=-6, ymin=0, ymax=3, zmin=0, zmax=19)
        root.addObject('BoxROI', name="box", box=[-10, -1, -0.0001, -5, 4, 0.0001])
        root.addObject('FixedProjectiveConstraint', indices="@box.indices")
        root.addObject('HexahedronFEMForceField', name="FEM", youngModulus=4000, poissonRatio=0.3, method="large")

        return root


    def main():
        root = Sofa.Core.Node("root")
        createScene(root)
        Sofa.Simulation.initRoot(root)

        for iteration in range(10):
            Sofa.Simulation.animate(root, root.dt.value)

        print("Simulation is done.")

        print(f"Number of errors: {msg_handler.num_errors}")
        print(f"Number of warnings: {msg_handler.num_warnings}")
        print(f"Number of infos: {msg_handler.num_infos}")


    if __name__ == '__main__':
        main()
