import Sofa
import unittest


class VecId(unittest.TestCase):
    def test_coord(self):
        # Const Coord
        vec = Sofa.Core.ConstVecCoordId()
        self.assertEqual(vec.index, 0)
        self.assertEqual(vec.getIndex(), 0)
        self.assertEqual(vec.type, Sofa.Core.VecType.V_COORD)

        vec = Sofa.Core.ConstVecCoordId(15)
        self.assertEqual(vec.index, 15)
        self.assertEqual(vec.getIndex(), 15)
        self.assertEqual(vec.type, Sofa.Core.VecType.V_COORD)

        # Coord
        vec = Sofa.Core.VecCoordId()
        self.assertEqual(vec.index, 0)
        self.assertEqual(vec.getIndex(), 0)
        self.assertEqual(vec.type, Sofa.Core.VecType.V_COORD)

        vec = Sofa.Core.VecCoordId(15)
        self.assertEqual(vec.index, 15)
        self.assertEqual(vec.getIndex(), 15)
        self.assertEqual(vec.type, Sofa.Core.VecType.V_COORD)

    def test_deriv(self):
        # Const Deriv
        vec = Sofa.Core.ConstVecDerivId()
        self.assertEqual(vec.index, 0)
        self.assertEqual(vec.getIndex(), 0)
        self.assertEqual(vec.type, Sofa.Core.VecType.V_DERIV)

        vec = Sofa.Core.ConstVecDerivId(15)
        self.assertEqual(vec.index, 15)
        self.assertEqual(vec.getIndex(), 15)
        self.assertEqual(vec.type, Sofa.Core.VecType.V_DERIV)

        # Deriv
        vec = Sofa.Core.VecDerivId()
        self.assertEqual(vec.index, 0)
        self.assertEqual(vec.getIndex(), 0)
        self.assertEqual(vec.type, Sofa.Core.VecType.V_DERIV)

        vec = Sofa.Core.VecDerivId(15)
        self.assertEqual(vec.index, 15)
        self.assertEqual(vec.getIndex(), 15)
        self.assertEqual(vec.type, Sofa.Core.VecType.V_DERIV)

    def test_default_coord_vectors(self):
        # Position
        self.assertEqual(Sofa.Core.VecCoordId.position().index, 1)
        self.assertEqual(Sofa.Core.VecCoordId.position().getName(), "position(V_COORD)")
        self.assertEqual(Sofa.Core.ConstVecCoordId.position().index, 1)
        self.assertEqual(Sofa.Core.ConstVecCoordId.position().getName(), "position(V_COORD)")

        # restPosition
        self.assertEqual(Sofa.Core.VecCoordId.restPosition().index, 2)
        self.assertEqual(Sofa.Core.VecCoordId.restPosition().getName(), "restPosition(V_COORD)")
        self.assertEqual(Sofa.Core.ConstVecCoordId.restPosition().index, 2)
        self.assertEqual(Sofa.Core.ConstVecCoordId.restPosition().getName(), "restPosition(V_COORD)")

        # freePosition
        self.assertEqual(Sofa.Core.VecCoordId.freePosition().index, 3)
        self.assertEqual(Sofa.Core.VecCoordId.freePosition().getName(), "freePosition(V_COORD)")
        self.assertEqual(Sofa.Core.ConstVecCoordId.freePosition().index, 3)
        self.assertEqual(Sofa.Core.ConstVecCoordId.freePosition().getName(), "freePosition(V_COORD)")

        # resetPosition
        self.assertEqual(Sofa.Core.VecCoordId.resetPosition().index, 4)
        self.assertEqual(Sofa.Core.VecCoordId.resetPosition().getName(), "resetPosition(V_COORD)")
        self.assertEqual(Sofa.Core.ConstVecCoordId.resetPosition().index, 4)
        self.assertEqual(Sofa.Core.ConstVecCoordId.resetPosition().getName(), "resetPosition(V_COORD)")

        # Dynamic offset
        self.assertEqual(Sofa.Core.VecId.getFirstDynamicIndex(Sofa.Core.VecType.V_COORD), 5)
        self.assertEqual(Sofa.Core.ConstVecId.getFirstDynamicIndex(Sofa.Core.VecType.V_COORD), 5)

    def test_default_deriv_vectors(self):
        # velocity
        self.assertEqual(Sofa.Core.VecDerivId.velocity().index, 1)
        self.assertEqual(Sofa.Core.VecDerivId.velocity().getName(), "velocity(V_DERIV)")
        self.assertEqual(Sofa.Core.ConstVecDerivId.velocity().index, 1)
        self.assertEqual(Sofa.Core.ConstVecDerivId.velocity().getName(), "velocity(V_DERIV)")

        # resetVelocity
        self.assertEqual(Sofa.Core.VecDerivId.resetVelocity().index, 2)
        self.assertEqual(Sofa.Core.VecDerivId.resetVelocity().getName(), "resetVelocity(V_DERIV)")
        self.assertEqual(Sofa.Core.ConstVecDerivId.resetVelocity().index, 2)
        self.assertEqual(Sofa.Core.ConstVecDerivId.resetVelocity().getName(), "resetVelocity(V_DERIV)")

        # freeVelocity
        self.assertEqual(Sofa.Core.VecDerivId.freeVelocity().index, 3)
        self.assertEqual(Sofa.Core.VecDerivId.freeVelocity().getName(), "freeVelocity(V_DERIV)")
        self.assertEqual(Sofa.Core.ConstVecDerivId.freeVelocity().index, 3)
        self.assertEqual(Sofa.Core.ConstVecDerivId.freeVelocity().getName(), "freeVelocity(V_DERIV)")

        # normal
        self.assertEqual(Sofa.Core.VecDerivId.normal().index, 4)
        self.assertEqual(Sofa.Core.VecDerivId.normal().getName(), "normal(V_DERIV)")
        self.assertEqual(Sofa.Core.ConstVecDerivId.normal().index, 4)
        self.assertEqual(Sofa.Core.ConstVecDerivId.normal().getName(), "normal(V_DERIV)")

        # force
        self.assertEqual(Sofa.Core.VecDerivId.force().index, 5)
        self.assertEqual(Sofa.Core.VecDerivId.force().getName(), "force(V_DERIV)")
        self.assertEqual(Sofa.Core.ConstVecDerivId.force().index, 5)
        self.assertEqual(Sofa.Core.ConstVecDerivId.force().getName(), "force(V_DERIV)")

        # externalForce
        self.assertEqual(Sofa.Core.VecDerivId.externalForce().index, 6)
        self.assertEqual(Sofa.Core.VecDerivId.externalForce().getName(), "externalForce(V_DERIV)")
        self.assertEqual(Sofa.Core.ConstVecDerivId.externalForce().index, 6)
        self.assertEqual(Sofa.Core.ConstVecDerivId.externalForce().getName(), "externalForce(V_DERIV)")

        # dx
        self.assertEqual(Sofa.Core.VecDerivId.dx().index, 7)
        self.assertEqual(Sofa.Core.VecDerivId.dx().getName(), "dx(V_DERIV)")
        self.assertEqual(Sofa.Core.ConstVecDerivId.dx().index, 7)
        self.assertEqual(Sofa.Core.ConstVecDerivId.dx().getName(), "dx(V_DERIV)")

        # dforce
        self.assertEqual(Sofa.Core.VecDerivId.dforce().index, 8)
        self.assertEqual(Sofa.Core.VecDerivId.dforce().getName(), "dforce(V_DERIV)")
        self.assertEqual(Sofa.Core.ConstVecDerivId.dforce().index, 8)
        self.assertEqual(Sofa.Core.ConstVecDerivId.dforce().getName(), "dforce(V_DERIV)")

        # Dynamic offset
        self.assertEqual(Sofa.Core.VecId.getFirstDynamicIndex(Sofa.Core.VecType.V_DERIV), 9)
        self.assertEqual(Sofa.Core.ConstVecId.getFirstDynamicIndex(Sofa.Core.VecType.V_DERIV), 9)

    def test_default_matderiv_vectors(self):
        # constraintJacobian
        self.assertEqual(Sofa.Core.MatrixDerivId.constraintJacobian().index, 1)
        self.assertEqual(Sofa.Core.MatrixDerivId.constraintJacobian().getName(), "holonomic(V_MATDERIV)")
        self.assertEqual(Sofa.Core.ConstMatrixDerivId.constraintJacobian().index, 1)
        self.assertEqual(Sofa.Core.ConstMatrixDerivId.constraintJacobian().getName(), "holonomic(V_MATDERIV)")

        # mappingJacobian
        self.assertEqual(Sofa.Core.MatrixDerivId.mappingJacobian().index, 2)
        self.assertEqual(Sofa.Core.MatrixDerivId.mappingJacobian().getName(), "nonHolonomic(V_MATDERIV)")
        self.assertEqual(Sofa.Core.ConstMatrixDerivId.mappingJacobian().index, 2)
        self.assertEqual(Sofa.Core.ConstMatrixDerivId.mappingJacobian().getName(), "nonHolonomic(V_MATDERIV)")

        # Dynamic offset
        self.assertEqual(Sofa.Core.VecId.getFirstDynamicIndex(Sofa.Core.VecType.V_MATDERIV), 3)
        self.assertEqual(Sofa.Core.ConstVecId.getFirstDynamicIndex(Sofa.Core.VecType.V_MATDERIV), 3)
