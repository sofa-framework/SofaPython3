import Sofa
import unittest


class Test(unittest.TestCase):
    def test_creation(self):
        mparams = Sofa.Core.MechanicalParams()

        # dt
        mparams.dt = 1.25
        self.assertEqual(mparams.dt, 1.25)
        mparams.setDt(1.50)
        self.assertEqual(mparams.dt, 1.50)

        # Implicit
        mparams.implicit = True
        self.assertTrue(mparams.implicit)
        mparams.setImplicit(False)
        self.assertFalse(mparams.implicit)

        # Mass factor
        mparams.mFactor = 1.30
        self.assertEqual(mparams.mFactor, 1.30)
        mparams.setMFactor(1.35)
        self.assertEqual(mparams.mFactor, 1.35)

        # Damping factor
        mparams.bFactor = 1.35
        self.assertEqual(mparams.bFactor, 1.35)
        mparams.setBFactor(1.40)
        self.assertEqual(mparams.bFactor, 1.40)

        # Stiffness factor
        mparams.kFactor = 1.45
        self.assertEqual(mparams.kFactor, 1.45)
        mparams.setKFactor(1.50)
        self.assertEqual(mparams.kFactor, 1.50)

        # Symmetric
        mparams.symmetricMatrix = True
        self.assertTrue(mparams.symmetricMatrix)
        mparams.setSymmetricMatrix(False)
        self.assertFalse(mparams.symmetricMatrix)

        # Energy
        mparams.energy = True
        self.assertTrue(mparams.energy)
        mparams.setEnergy(False)
        self.assertFalse(mparams.energy)
