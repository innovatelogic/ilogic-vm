#include "mathlib.h"
#include <gtest/gtest.h>

namespace oes
{
    namespace common_base
    {
        TEST(Math, matrix_inverse)
        {
            // A * (A)^-1 = I
            Matrix A;
            Vector n(1.f, 1.f, 1.f);
            n.normalize();
            A.SetRotateAxis(n, m_pi * 0.5f);
            
            Matrix A_Inv = A.getInverse3x3();
            
            Matrix MInv = A * A_Inv;
            Matrix I;
            EXPECT_TRUE(MInv.equal(I));
        }

        TEST(Math, Vector_matrix_chain_transform)
        {
            //      root
            //       |
            //     child0
            //       |
            //     child2

            Matrix root;
            Matrix child0;
            Matrix child1;

            root.Identitly();
            child0.SetRotateAxis(Vector(1.f, 0.f, 0.f), m_pi);

        }
    }
}