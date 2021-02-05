import Sofa
import Sofa.SofaGL
import Sofa.Gui
import SofaRuntime
import Sofa.Simulation as sim
import os
import time
sofa_directory = os.environ['SOFA_ROOT']
import pygame
from OpenGL.GL import *
from OpenGL.GLU import *


pygame.display.init()
display_size = (800, 600)
pygame.display.set_mode(display_size, pygame.DOUBLEBUF | pygame.OPENGL)

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
glEnable(GL_LIGHTING)
glEnable(GL_DEPTH_TEST)
Sofa.SofaGL.glewInit()
glMatrixMode(GL_PROJECTION)
glLoadIdentity()
gluPerspective(45, (display_size[0] / display_size[1]), 0.1, 50.0)

glMatrixMode(GL_MODELVIEW)
glLoadIdentity()

def simple_render(rootNode):
    """
     Get the OpenGL Context to render an image (snapshot) of the simulation state
     """
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glEnable(GL_LIGHTING)
    glEnable(GL_DEPTH_TEST)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45, (display_size[0] / display_size[1]), 0.1, 50.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

    cameraMVM = rootNode.visuals.camera.getOpenGLModelViewMatrix()
    glMultMatrixd(cameraMVM)

    Sofa.SofaGL.draw(rootNode.visuals)

    pygame.display.get_surface().fill((0,0,0))
    pygame.display.flip()


class scene_interface:
    """Scene_interface provides step and reset methods"""

    def __init__(self, max_steps=1000):
        # max_steps, how long the simulator should run. Total length: dt*max_steps
        self.max_steps = max_steps

        # the current step in the simulation
        self.current_step = 0
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

        self.root = Sofa.Core.Node("myroot")

        ### create some objects to observe
        self.place_objects_in_scene(self.root)

        # place light and a camera
        self.visuals = self.root.addChild('visuals')
        self.visuals.addObject("LightManager")
        self.visuals.addObject("SpotLight", position=[0,10,0], direction=[0,-1,0])
        self.visuals.addObject("InteractiveCamera", name="camera", position=[0,10, 0],
                            lookAt=[0,0,0], distance=37,
                            fieldOfView=45, zNear=0.63, zFar=55.69)

        # start the simulator
        Sofa.Simulation.init(self.root)

    def place_objects_in_scene(self, root):
        ### these are just some things that stay still and move around
        # so you know the animation is actually happening
        root.gravity = [0, -1., 0]
        root.addObject("VisualStyle", displayFlags="showBehaviorModels showAll")
        root.addObject("MeshGmshLoader", name="meshLoaderCoarse",
                       filename="mesh/liver.msh")
        root.addObject("MeshObjLoader", name="meshLoaderFine",
                       filename="mesh/liver-smooth.obj")

        root.addObject("EulerImplicitSolver")
        root.addObject("CGLinearSolver", iterations="200",
                        tolerance="1e-09", threshold="1e-09")

        liver = root.addChild("liver")

        liver.addObject("TetrahedronSetTopologyContainer",
                        name="topo", src="@../meshLoaderCoarse" )
        liver.addObject("TetrahedronSetGeometryAlgorithms",
                        template="Vec3d", name="GeomAlgo")
        liver.addObject("MechanicalObject",
                        template="Vec3d",
                        name="MechanicalModel", showObject="1", showObjectScale="3")

        liver.addObject("TetrahedronFEMForceField", name="fem", youngModulus="1000",
                        poissonRatio="0.4", method="large")

        liver.addObject("MeshMatrixMass", massDensity="1")
        liver.addObject("FixedConstraint", indices="2 3 50")

    def step(self):
        # this steps the simulation
        Sofa.Simulation.animate(self.root, self.root.getDt())
        self.visuals.camera.position = self.visuals.camera.position + [-0.002, 0, 0]
        simple_render(self.root)

        # just to keep track of where we are
        self.current_step += 1

        # return true if done
        return self.current_step >= self.max_steps


if __name__ == '__main__':
    a = scene_interface()
    done = False
    while not done:
        factor = a.current_step
        done = a.step()
        time.sleep(a.root.getDt())
