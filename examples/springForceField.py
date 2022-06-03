import Sofa
import SofaRuntime
from Sofa import SofaDeformable


def createScene(root):
    root.gravity = [0, -9.81, 0]

    root.addObject('DefaultAnimationLoop')
    root.addObject('DefaultVisualManagerLoop')

    surface_node = root.addChild('Surface')
    surface_loader = surface_node.addObject('MeshOBJLoader', name='surface_loader', filename='mesh/ball.obj')

    root.addObject('EulerImplicitSolver', name='ODE', rayleighStiffness=0.1, rayleighMass=0.1)
    root.addObject('CGLinearSolver', name='linear solver', iterations=25, tolerance=1e-08, threshold=1e-08)

    ball_0 = add_ball(root, 'ball_0', (-2, 0, 0), surface_loader)
    ball_1 = add_ball(root, 'ball_1', (2, 0, 0), surface_loader)

    ball_0.addObject('RestShapeSpringsForceField', name='rest_spring', stiffness=1e1, angularStiffness=1e03)
    springs = root.addObject('StiffSpringForceField',
                             object1=ball_0.dofs.getLinkPath(), object2=ball_1.dofs.getLinkPath(),
                             indices1=[0], indices2=[0],
                             length=[4], stiffness=2)

    root.addObject(KeyPressedController(name='controller', springs=springs))

    floor = root.addChild('Floor')
    floor_loader = floor.addObject('MeshOBJLoader', name='surface_loader', filename='mesh/floor.obj',
                                    scale3d=[0.2, 0.1, 0.2], translation=[0, -15, 0])
    floor.addObject('OglModel', src=floor_loader.getLinkPath())


def add_ball(root, name, position, obj_loader):
    ball_node = root.addChild(name)

    initial_position = position + (0, 0, 0, 1)
    ball_node.addObject('MechanicalObject', name='dofs', template='Rigid3d', position=initial_position)
    ball_node.addObject('UniformMass', name='mass', totalMass=1)
    ball_node.addObject('PlaneForceField', normal='0 1 0', d='-15', stiffness='10000')

    visual_model = ball_node.addChild('visual')
    visual_model.addObject('OglModel', src=obj_loader.getLinkPath())
    visual_model.addObject('RigidMapping')

    return ball_node


class KeyPressedController(Sofa.Core.Controller):

    def __init__(self, *args, **kwargs):
        Sofa.Core.Controller.__init__(self, *args, **kwargs)
        self.springs = kwargs.get('springs')

    def onKeypressedEvent(self, event):
        if event['key'] == 'L':
            print('Remove spring')
            self.springs.removeSpring(0)
