from MyRestShapeForceField import *


def createIntegrationScheme(node, use_implicit_scheme):
    if use_implicit_scheme is True:
        node.addObject('EulerImplicitSolver', name='odeImplicitSolver',
                       rayleighStiffness='0.1', rayleighMass='0.1')
    else:
        node.addObject('EulerExplicitSolver', name='odeExplicitSolver')


def createSolver(node, use_iterative_solver):
    if use_iterative_solver is True:
        node.addObject('CGLinearSolver', name='linearSolver',
                       iterations=30, tolerance=1.e-9, threshold=1.e-9)
    else:
        node.addObject('SparseLDLSolver', name='ldlSolver')


def createDragon(node, node_name, use_implicit_scheme, use_iterative_solver):
    dragon = node.addChild(node_name)
    createIntegrationScheme(dragon, use_implicit_scheme)
    createSolver(dragon, use_iterative_solver)
    dragon.addObject('SparseGridTopology', n=[
                     10, 5, 10], fileTopology="mesh/dragon.obj")
    dofs = dragon.addObject(
        'MechanicalObject', name="DOFs")
    dragon.addObject('UniformMass', totalMass=1.0)

    print (str(dofs.rest_position))
    myRSSFF = NaiveRestShapeSpringsForcefield(name="Springs",
                                           stiffness=50,
                                           mstate=dofs, rest_pos=dofs.rest_position, implementation="naive")
    dragon.addObject(myRSSFF)

    visu = dragon.addChild("Visu")
    vm = visu.addObject('OglModel', fileMesh="mesh/dragon.obj",
                        color=[1.0, 0.5, 1.0, 1.0])
    visu.addObject('BarycentricMapping',
                   input=dofs.getLinkPath(), output=vm.getLinkPath())

    collision = dragon.addChild("Collis")
    collision.addObject('MeshObjLoader', name="loader",
                        filename="mesh/dragon.obj")
    collision.addObject('Mesh', src="@loader")
    collision.addObject('MechanicalObject', src="@loader")
    collision.addObject('TriangleCollisionModel', group="1")
    collision.addObject('LineCollisionModel', group="1")
    collision.addObject('PointCollisionModel', group="1")
    collision.addObject('BarycentricMapping', input="@..", output="@.")


def createCube(node, node_name, use_implicit_scheme, use_iterative_solver):
    cube = node.addChild(node_name)
    createIntegrationScheme(cube, use_implicit_scheme)
    createSolver(cube, use_iterative_solver)
    cube.addObject('SparseGridTopology', n=[
                   2, 2, 2], fileTopology="mesh/smCube27.obj")
    dofs = cube.addObject('MechanicalObject', name="DOFs", dy=20)
    cube.addObject('UniformMass', totalMass=1.0)

    myRSSFF = NaiveRestShapeSpringsForcefield(name="Springs",
                                           stiffness=50,
                                           mstate=dofs, rest_pos=dofs.rest_position, implementation="naive")
    cube.addObject(myRSSFF)

    visu = cube.addChild("Visu")
    vm = visu.addObject(
        'OglModel', fileMesh="mesh/smCube27.obj", color=[0.5, 1.0, 0.5, 1.0])
    visu.addObject('BarycentricMapping',
                   input=dofs.getLinkPath(), output=vm.getLinkPath())

    collision = cube.addChild("Collis")
    collision.addObject('MeshObjLoader', name="loader",
                        filename="mesh/smCube27.obj", triangulate=True)
    collision.addObject('Mesh', src="@loader")
    collision.addObject('MechanicalObject', src="@loader")
    collision.addObject('TriangleCollisionModel', group="1")
    collision.addObject('LineCollisionModel', group="1")
    collision.addObject('PointCollisionModel', group="1")
    collision.addObject('BarycentricMapping', input="@..", output="@.")


def createScene(root):
    root.gravity = [0, -10, 0]
    createDragon(root, "Dragon", True, False)
    createCube(root, "Cube", True, True)
    return root
