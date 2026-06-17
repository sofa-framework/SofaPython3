import math
import unittest

import Sofa
from SofaTypes import Mat1x1, Mat2x2, Mat3x3, Vec2d

class TestMaterialMatrix(unittest.TestCase):

    def test_Constructors(self):
        # --- Mat1x1 (Scalar/Row Vector) Construction ---
        m = Mat1x1() # Empty constructor
        self.assertEqual(m[0][0], 0.0)

        m = Mat1x1([[math.pi]]) # list ctor (or direct assignment)
        self.assertEqual(m[0][0], math.pi)

        # --- Mat2x2 Construction ---
        m = Mat2x2([[1.0, 2.0], [3.0, 4.0]]) # list of lists ctor
        self.assertEqual(m[0][0], 1.0)
        self.assertEqual(m[0][1], 2.0)
        self.assertEqual(m[1][0], 3.0)
        self.assertEqual(m[1][1], 4.0)

        # --- Mat3x3 Construction ---
        m = Mat3x3([[1.0, 2.0, 3.0], [4.0, 5.0, 6.0], [7.0, 8.0, 9.0]])
        self.assertEqual(m[0][0], 1.0)
        self.assertEqual(m[0][1], 2.0)
        self.assertEqual(m[0][2], 3.0)
        self.assertEqual(m[1][0], 4.0)
        self.assertEqual(m[1][1], 5.0)
        self.assertEqual(m[1][2], 6.0)
        self.assertEqual(m[2][0], 7.0)
        self.assertEqual(m[2][1], 8.0)
        self.assertEqual(m[2][2], 9.0)

    def test_Operators(self):
        m1 = Mat2x2([[1.0, 2.0], [3.0, 4.0]])
        m2 = Mat2x2([[5.0, 6.0], [7.0, 8.0]])

        # Matrix addition
        sum_mat = m1 + m2
        expected_sum = Mat2x2([[6.0, 8.0], [10.0, 12.0]])
        self.assertEqual(sum_mat, expected_sum)

        # Matrix subtraction
        diff_mat = m1 - m2
        expected_diff = Mat2x2([[-4.0, -4.0], [-4.0, -4.0]])
        self.assertEqual(diff_mat, expected_diff)

        # Scalar multiplication (A * 2.0)
        scaled_mat = m1 * 2.0
        expected_scale = Mat2x2([[2.0, 4.0], [6.0, 8.0]])
        self.assertEqual(scaled_mat, expected_scale)

        # Matrix multiplication (A * B)
        mat_product = m1 * m2
        expected_mat_product = Mat2x2([[1.0 * 5.0 + 2.0 * 7.0, 1.0 * 6.0 + 2.0 * 8.0], [3.0 * 5.0 + 4.0 * 7.0, 3.0 * 6.0 + 4.0 * 8.0]])
        self.assertEqual(mat_product, expected_mat_product)

        # Testing assignment/in-place modification if available (e.g., /=)
        m_test = Mat2x2([[1.0, 2.0], [3.0, 4.0]])
        m_test /= 2.0
        expected_inplace_div = Mat2x2([[0.5, 1.0], [1.5, 2.0]])
        self.assertEqual(m_test, expected_inplace_div)
