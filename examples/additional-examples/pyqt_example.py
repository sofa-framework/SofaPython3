from qtpy.QtCore import *
from qtpy.QtWidgets import *
from qtpy.QtOpenGL import *
import Sofa.SofaGL
import Sofa
import SofaRuntime
import Sofa.Simulation as sim
import os
sofa_directory = os.environ['SOFA_ROOT']
from OpenGL.GL import *
from OpenGL.GLU import *
import numpy as np
from PIL import Image

"""
With something like this setup, we can use Sofa with our own GUI and not have to give over control of the main thread.
Simple pyqt signals can be added to manually rotate the view with the mouse (could be a tedious task to get it to
be intuitive).
"""

class MainWindow(QWidget):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.sofa_sim = SofaSim()
        self.sofa_sim.init_sim()
        self.sofa_view = glSofaWidget(self.sofa_sim.visuals)
        mainLayout = QHBoxLayout()
        mainLayout.addWidget(self.sofa_view)
        self.setLayout(mainLayout)
        self.simulation_timer = QTimer()
        self.simulation_timer.timeout.connect(self.step_sim)
        self.simulation_timer.setInterval(self.sofa_sim.root.getDt())
        self.simulation_timer.start()

    def step_sim(self):
        self.sofa_sim.step_sim()
        self.sofa_view.update()

    def keyPressEvent(self, QKeyEvent):
        if QKeyEvent.key() == Qt.Key_Space:
            self.sofa_view.get_depth_image()


class glSofaWidget(QGLWidget):
    def __init__(self, sofa_visuals_node):
        QGLWidget.__init__(self)
        self.visuals_node = sofa_visuals_node
        self.setMinimumSize(800, 600)
        self.z_far = sofa_visuals_node.camera.findData('zFar').value
        self.z_near = sofa_visuals_node.camera.findData('zNear').value

    def initializeGL(self):
        glViewport(0,0, self.width(), self.height())
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glEnable(GL_LIGHTING)
        glEnable(GL_DEPTH_TEST)
        glDepthFunc(GL_LESS)
        Sofa.SofaGL.glewInit()
        Sofa.Simulation.initVisual(self.visuals_node)
        Sofa.Simulation.initTextures(self.visuals_node)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(45, (self.width() / self.height()), 0.1, 50.0)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()

    def paintGL(self):
        glViewport(0,0, self.width(), self.height())
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(45, (self.width()/ self.height()), 0.1, 50.0)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()

        cameraMVM = self.visuals_node.camera.getOpenGLModelViewMatrix()
        glMultMatrixd(cameraMVM)

        Sofa.SofaGL.draw(self.visuals_node)

    def get_depth_image(self):
        _, _, width, height = glGetIntegerv(GL_VIEWPORT)
        buff = glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT)
        image = np.frombuffer(buff, dtype=np.float32)
        image = image.reshape(height, width)
        image = np.flipud(image)  #<-- image is now a numpy array you can use
        depth_image = -self.z_far*self.z_near/(self.z_far + image*(self.z_near-self.z_far))
        depth_image = (depth_image - depth_image.min()) / (depth_image.max() - depth_image.min())
        depth_image = depth_image * 255

        img2 = Image.fromarray(depth_image.astype(np.uint8), 'L')
        img2.show()


class SofaSim():
    def __init__(self):
        # Register all the common component in the factory.
        SofaRuntime.PluginRepository.addFirstPath(os.path.join(sofa_directory, 'bin'))
        SofaRuntime.importPlugin('SofaOpenglVisual')
        SofaRuntime.importPlugin("SofaComponentAll")
        SofaRuntime.importPlugin("SofaGeneralLoader")
        SofaRuntime.importPlugin("SofaImplicitOdeSolver")
        SofaRuntime.importPlugin("SofaLoader")
        SofaRuntime.importPlugin("SofaSimpleFem")
        SofaRuntime.importPlugin("SofaBoundaryCondition")
        SofaRuntime.importPlugin("SofaMiscForceField")
        self.root = Sofa.Core.Node("Root")
        root = self.root
        root.gravity = [0, -1., 0]
        root.addObject("VisualStyle", displayFlags="showBehaviorModels showAll")
        root.addObject("MeshGmshLoader", name="meshLoaderCoarse",
                       filename="mesh/liver.msh")
        root.addObject("MeshOBJLoader", name="meshLoaderFine",
                       filename="mesh/liver-smooth.obj")

        root.addObject("EulerImplicitSolver")
        root.addObject("CGLinearSolver", iterations="200",
                       tolerance="1e-09", threshold="1e-09")

        liver = root.addChild("liver")

        liver.addObject("TetrahedronSetTopologyContainer",
                        name="topo", src="@../meshLoaderCoarse")
        liver.addObject("TetrahedronSetGeometryAlgorithms",
                        template="Vec3d", name="GeomAlgo")
        liver.addObject("MechanicalObject",
                        template="Vec3d",
                        name="MechanicalModel", showObject="1", showObjectScale="3")

        liver.addObject("TetrahedronFEMForceField", name="fem", youngModulus="1000",
                        poissonRatio="0.4", method="large")

        liver.addObject("MeshMatrixMass", massDensity="1")
        liver.addObject("FixedConstraint", indices="2 3 50")

        visual = liver.addChild("visual")
        visual.addObject('MeshOBJLoader', name="meshLoader_0", filename="mesh/liver-smooth.obj", handleSeams="1")
        visual.addObject('OglModel', name="VisualModel", src="@meshLoader_0", color='red')
        visual.addObject('BarycentricMapping', input="@..", output="@VisualModel", name="visual mapping")

        # place light and a camera
        self.visuals = root.addChild('visuals')
        root.addObject("LightManager")
        root.addObject("DirectionalLight", direction=[0, 1, 0])
        self.visuals.addObject("InteractiveCamera", name="camera", position=[0, 15, 0],
                               lookAt=[0, 0, 0], distance=37,
                               fieldOfView=45, zNear=0.63, zFar=55.69)

    def init_sim(self):
        # start the simulator
        Sofa.Simulation.init(self.root)

    def step_sim(self):
        self.visuals.camera.position = self.visuals.camera.position + [-0.0002, 0, 0]
        Sofa.Simulation.animate(self.root, self.root.getDt())  # uncomment to animated sim
        Sofa.Simulation.updateVisual(self.root)


if __name__ == '__main__':
    app = QApplication(['Yo'])
    window = MainWindow()
    window.show()
    app.exec_()