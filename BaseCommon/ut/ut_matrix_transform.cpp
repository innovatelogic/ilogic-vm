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
            //                          transform
            //      root
            //       |                          Y        
            //     child0                       |      Z
            //       |                          |    /
            //     child1                       |   /
            //       |                          |  /
            //     child2                        ____________ X
            //       |    
            //    [loc_pos]
            //
            //      

            Matrix root;
            Matrix child0;
            Matrix child1;
            Matrix child2;

            root.Identitly();
            child0.SetRotateAxis(Vector(1.f, 0.f, 0.f), m_pi * 0.5f); // X->X; Y->-Z; Z->Y 
            child1.SetRotateAxis(Vector(0.f, 1.f, 0.f), m_pi * 0.5f);
            child2.SetRotateAxis(Vector(0.f, 0.f, 1.f), m_pi * 0.5f);

            Matrix world = root * child0 * child1 * child2;

            Vector loc_pos(1.f, 0.f, 0.f);

            Vector vworld;
            transform_coord(vworld, loc_pos, world);

            //EXPECT_EQ(world.row0, vlocal);
        }
    }
}