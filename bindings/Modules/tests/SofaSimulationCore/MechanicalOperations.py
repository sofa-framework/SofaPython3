import unittest
import Sofa
from Sofa import SofaSimulationCore
import numpy as np

"""
In order to test out a good subset of the Mechanical operations, let's do here do a Newton-Raphson solver in python.
"""


class StaticOdeSolver (Sofa.Core.OdeSolver):
    def solve(self, _, dt, X, V):
        mparams = Sofa.Core.MechanicalParams()
        vop = SofaSimulationCore.VectorOperations(self.getContext())
        mop = SofaSimulationCore.MechanicalOperations(mparams, self.getContext())

        # Allocate the solution vector
        dx = Sofa.Core.VecId.dx()
        vop.v_realloc(dx, interactionForceField=True, propagate=True)
        vop.v_clear(dx)

        # Compute the residual
        F = Sofa.Core.VecId.force()
        mop.computeForce(result=F, clear=True, accumulate=True);
        mop.projectResponse(F)
        vop.v_dot(F, F)
        F_norm = np.sqrt(vop.finish())

        # Assemble the system
        mop.m_setSystemMBKMatrix(m=0, b=0, k=-1)
        mop.m_setSystemRHVector(F)
        mop.m_setSystemLHVector(dx)

        # Solve the system
        mop.m_solveSystem()
        vop.v_dot(dx, dx)
        dx_norm = np.sqrt(vop.finish())

        # Propagate the solution
        vop.v_peq(X, dx)  # X += dx

        # Solve the constraints
        # todo: Bind ConstraintParams
        # mop.solveConstraint(X, ConstraintParams.ConstOrder.POS)

        print(f"Solved with |F| = {F_norm} and |dx| = {dx_norm}")


class MechanicalOperations(unittest.TestCase):
    def test_static_solver(self):
        root = Sofa.Core.Node()
        createScene(root)
        Sofa.Simulation.init(root)
        for _ in range(5):
            Sofa.Simulation.animate(root, root.dt.value)

        middle_node_index = 52
        solution_of_middle_node = np.array([0, 10.0953, -0.285531])
        self.assertLess(np.linalg.norm(solution_of_middle_node - root.mechanics.neumann.mo.position.array()[middle_node_index])/np.linalg.norm(solution_of_middle_node), 1e-5)


def createScene(root):
    w, l = 2, 10
    nx, ny = 3, 9
    dx, dy = w/nx/2, l/ny/2
    root.addObject('RequiredPlugin', pluginName='SofaLoader SofaBoundaryCondition SofaEngine SofaSimpleFem SofaSparseSolver SofaTopologyMapping')
    root.addObject('MeshObjLoader', name='surface', filename='mesh/cylinder.obj')
    root.addObject('SparseGridTopology', src='@surface', name='grid', n=[nx, ny, nx])
    root.addChild('mechanics')
    root.mechanics.addObject(StaticOdeSolver(name='solver'))
    root.mechanics.addObject('SparseLDLSolver')
    root.mechanics.addObject('MechanicalObject', name='mo', src='@../grid')
    root.mechanics.addObject('HexahedronSetTopologyContainer', name='hexa_topology', hexahedra='@../grid.hexahedra')
    root.mechanics.addObject('HexahedronFEMForceField', youngModulus=3000, poissonRatio=0.3)
    root.mechanics.addObject('BoxROI', name='fixed_roi', box=[-1-dx,0-dx,-1-dx, 1+dx, 0+dx, 1+dx], drawBoxes=True)
    root.mechanics.addObject('FixedConstraint',  indices='@fixed_roi.indices')
    root.mechanics.addChild('neumann')
    root.mechanics.neumann.addObject('MechanicalObject', name='mo')
    root.mechanics.neumann.addObject('QuadSetTopologyContainer', name='quad_topology')
    root.mechanics.neumann.addObject('QuadSetTopologyModifier')
    root.mechanics.neumann.addObject('QuadSetGeometryAlgorithms')
    root.mechanics.neumann.addObject('Hexa2QuadTopologicalMapping', input='@../hexa_topology', output='@quad_topology')
    root.mechanics.neumann.addObject('SubsetMapping', applyRestPosition=True, input='@../mo', output='@./mo', indices='@quad_topology.points')
    root.mechanics.neumann.addObject('BoxROI', name='top_roi', quad='@quad_topology.quads', src='@mo', box=[-1-dx,10-dx,-1-dx, 1+dx, 10+dx, 1+dx], drawBoxes=True)
    root.mechanics.neumann.addObject('QuadPressureForceField', pressure=[0, 0, -1], quadList='@top_roi.quadIndices')
