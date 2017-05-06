#include "mathlib.h"
#include <gtest/gtest.h>

namespace oes
{
    namespace common_base
    {
        //----------------------------------------------------------------------------------------------
        TEST(Math, matrix_inverse)
        {
            // A * (A)^-1 = I
            Matrix A;
            Vector n(1.f, 1.f, 1.f);
            n.normalize();
            A.SetRotateAxis(n, m_pi * 0.5f);
            A.set_translation(Vector(5.f, 5.f, 5.f));
            
            Matrix A_Inv = A.inverse();
            
            Matrix MInv = A * A_Inv;
            Matrix I;
            EXPECT_TRUE(MInv.equal(I));
        }

        //----------------------------------------------------------------------------------------------
        TEST(Math, Vector_matrix_chain_transform)
        {
            //                          transform
            //          root
            //            |                      Y        
            //       A        C                  |      Z
            //       |        |                  |    /
            //       B    [c: loc_pos to b?]     |   /
            //       |                           |  /
            //   [b:loc_pos]                       ____________ X
            //           
            //    b' = [root*A*B] * b
            //    c' = [root*C] * c
            //    b' == c'
            //    [root*C] * c = [root*A*B] * b => 
            //    c = [root*A*B] * [root*C]^t * b

            for (int i = 0; i < 65; i++) // > 66 iterations is precision lost on win x86 
            {
                Matrix root;
                Matrix A;
                Matrix B;
                Matrix C;

                root.Identitly();
                A.SetRotateAxis(Vector(1.f, 0.f, 0.f), m_pi * 0.5f * i); // X->X; Y->-Z; Z->Y 
                A.t = Vector(i, -i, 0.f);

                B.SetRotateAxis(Vector(0.f, 1.f, 0.f), m_pi * 0.5f * i);
                B.t = Vector(0.f, i, -i);

                C.SetRotateAxis(Vector(0.f, 0.f, 1.f), m_pi * 0.5f * i);
                C.t = Vector(0.f, 0.f, i);

                Matrix root_AB = root * A * B;
                Matrix root_C = root * C;

                Matrix int_root_C = root_C.inverse();

                Vector b(1.f * (i % 2 ? 1 : -1), 1.f * (i % 2 ? -1 : 1), 1.f * (i % 2 ? 0.f : 1));

                Vector c;
                transform_coord(c, b, root_AB * int_root_C);

                Vector cw;
                transform_coord(cw, c, root_C);

                Vector bw;
                transform_coord(bw, b, root_AB);

                bool flag = cw.equal(bw);
                if (!flag) {
                    int k = 0;
                }
                EXPECT_TRUE(cw.equal(bw));
            }
        }
    }
}