import unittest
import Sofa
from Sofa import SofaSimulationCore
import numpy as np


class VectorOperations(unittest.TestCase):
    def test_alloc_coord(self):
        root = Sofa.Core.Node()
        create_scene(root)
        Sofa.Simulation.init(root)

        vop = SofaSimulationCore.VectorOperations(root)

        # Create a first multivector
        multi_vec_id_1 = Sofa.Core.MultiVecCoordId()
        self.assertEqual(multi_vec_id_1.defaultId.index, 0)
        self.assertTrue(multi_vec_id_1.isNull())
        vop.v_alloc(multi_vec_id_1)
        self.assertFalse(multi_vec_id_1.isNull())
        self.assertEqual(multi_vec_id_1.getId(root.node_1.mo).index, 5)
        self.assertEqual(multi_vec_id_1.getId(root.node_2.mo).index, 5)

        # Create a second one
        multi_vec_id_2 = Sofa.Core.MultiVecCoordId()
        vop.v_alloc(multi_vec_id_2)
        self.assertEqual(multi_vec_id_2.getId(root.node_1.mo).index, 6)
        self.assertEqual(multi_vec_id_2.getId(root.node_2.mo).index, 6)

        # Create a third one, only in node_2
        vop2 = SofaSimulationCore.VectorOperations(root.node_2)
        multi_vec_id_3 = Sofa.Core.MultiVecCoordId()
        vop2.v_alloc(multi_vec_id_3)
        self.assertEqual(multi_vec_id_3.getId(root.node_1.mo).index, 0)
        self.assertEqual(multi_vec_id_3.getId(root.node_2.mo).index, 7)

        # Free
        vop.v_free(multi_vec_id_1)
        vop.v_free(multi_vec_id_2)
        vop2.v_free(multi_vec_id_3)

    def test_alloc_deriv(self):
        root = Sofa.Core.Node()
        create_scene(root)
        Sofa.Simulation.init(root)

        vop = SofaSimulationCore.VectorOperations(root)

        # Create a first multivector
        multi_vec_id_1 = Sofa.Core.MultiVecDerivId()
        self.assertEqual(multi_vec_id_1.defaultId.index, 0)
        self.assertTrue(multi_vec_id_1.isNull())
        vop.v_alloc(multi_vec_id_1)
        self.assertFalse(multi_vec_id_1.isNull())
        self.assertEqual(multi_vec_id_1.getId(root.node_1.mo).index, 9)
        self.assertEqual(multi_vec_id_1.getId(root.node_2.mo).index, 9)

        # Create a second one
        multi_vec_id_2 = Sofa.Core.MultiVecDerivId()
        vop.v_alloc(multi_vec_id_2)
        self.assertEqual(multi_vec_id_2.getId(root.node_1.mo).index, 10)
        self.assertEqual(multi_vec_id_2.getId(root.node_2.mo).index, 10)

        # Create a third one, only in node_2
        vop2 = SofaSimulationCore.VectorOperations(root.node_2)
        multi_vec_id_3 = Sofa.Core.MultiVecDerivId()
        vop2.v_alloc(multi_vec_id_3)
        self.assertEqual(multi_vec_id_3.getId(root.node_1.mo).index, 0)
        self.assertEqual(multi_vec_id_3.getId(root.node_2.mo).index, 11)

        # Free
        vop.v_free(multi_vec_id_1)
        vop.v_free(multi_vec_id_2)
        vop2.v_free(multi_vec_id_3)

    def test_op(self):
        root = Sofa.Core.Node()
        create_scene(root)
        Sofa.Simulation.init(root)

        vop = SofaSimulationCore.VectorOperations(root)

        # Create a temporary multivector
        v = Sofa.Core.MultiVecCoordId()
        vop.v_alloc(v)

        x_id = Sofa.Core.VecCoordId.position()
        vop.v_eq(v, x_id)  # v = x
        vop.v_peq(v, x_id, 2)  # v += 2*x
        vop.v_dot(v, v) # n = sqrt(v dot v)
        n = np.sqrt(vop.finish())

        x = np.concatenate((root.node_1.mo.position.array(), root.node_2.mo.position.array()), axis=None).flatten()
        self.assertEqual(n, np.linalg.norm(x + (2*x)))


def create_scene(root):
    root.addObject('RequiredPlugin', pluginName='SofaBaseMechanics')
    root.addChild('node_1')
    root.node_1.addObject('MechanicalObject', name='mo', template='Vec3', position=[[1,1,1], [2,2,2], [3,3,3]])

    root.addChild('node_2')
    root.node_2.addObject('MechanicalObject', name='mo', template='Vec2', position=[[1,1], [2,2], [3,3], [4, 4]])
