import Sofa
import unittest


class MultiVecId(unittest.TestCase):
    """
    Test the creation of MultiVecId

    Note that this test case will not test the allocation inside mechanical object. This is instead tested inside
    the vector operations (Sofa.SimulationCore.VectorOperations) test case.
    """
    def test_coord(self):
        # Const Coord
        vec = Sofa.Core.ConstMultiVecCoordId()
        self.assertEqual(vec.defaultId.index, 0)
        self.assertEqual(vec.getDefaultId().getIndex(), 0)
        self.assertEqual(vec.defaultId.type, Sofa.Core.VecType.V_COORD)

        vec = Sofa.Core.ConstMultiVecCoordId(Sofa.Core.ConstVecCoordId(15))
        self.assertEqual(vec.defaultId.index, 15)
        self.assertEqual(vec.getDefaultId().getIndex(), 15)
        self.assertEqual(vec.defaultId.type, Sofa.Core.VecType.V_COORD)

        vec = Sofa.Core.ConstMultiVecCoordId(Sofa.Core.VecCoordId(15))
        self.assertEqual(vec.defaultId.index, 15)
        self.assertEqual(vec.getDefaultId().getIndex(), 15)
        self.assertEqual(vec.defaultId.type, Sofa.Core.VecType.V_COORD)

        # Coord
        vec = Sofa.Core.MultiVecCoordId()
        self.assertEqual(vec.defaultId.index, 0)
        self.assertEqual(vec.getDefaultId().getIndex(), 0)
        self.assertEqual(vec.defaultId.type, Sofa.Core.VecType.V_COORD)

        vec = Sofa.Core.MultiVecCoordId(Sofa.Core.VecCoordId(15))
        self.assertEqual(vec.defaultId.index, 15)
        self.assertEqual(vec.getDefaultId().getIndex(), 15)
        self.assertEqual(vec.defaultId.type, Sofa.Core.VecType.V_COORD)
