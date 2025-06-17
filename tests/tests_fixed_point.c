#include "jfp.h"
#include "jtest.h"
#include <stdio.h>

JT_TESTS{

JT_TEST("Assert test"){

    JT_ASSERT(2 == 2);
    JT_ASSERT(2 == 2);
    

}
JT_TEST("Conversion checks"){
    // Checking whether the epsilon converts to itself
    JT_ASSERT(
        SFP_10_5_TO_FLOAT(SFP_10_5_ONE*SFP_10_5_EPSILON) == SFP_10_5_EPSILON);

    // Check whether the maximum number is converted correctly.

    // Checks on any things that can be accurately represented.
    JT_ASSERT(SFP_10_5_TO_FLOAT(SFP_10_5_ONE) == 1.0f);
    JT_ASSERT(SFP_10_5_TO_FLOAT(FLOAT_TO_SFP_10_5(1.0f)) == 1.0f);
    
    JT_ASSERT(SFP_10_5_TO_FLOAT(SFP_10_5_ONE*-2) == -2.0f);
    JT_ASSERT(SFP_10_5_TO_FLOAT(FLOAT_TO_SFP_10_5(-2.0f)) == -2.0f);
    
    JT_ASSERT(SFP_10_5_TO_FLOAT(SFP_10_5_ONE*4) == 4.0f);
    JT_ASSERT(SFP_10_5_TO_FLOAT(FLOAT_TO_SFP_10_5(4.0f)) == 4.0f);

    JT_ASSERT(SFP_10_5_TO_FLOAT(SFP_10_5_ONE*0.5) == 0.5f);
    JT_ASSERT(SFP_10_5_TO_FLOAT(FLOAT_TO_SFP_10_5(0.5f)) == 0.5f);
    
    JT_ASSERT(SFP_10_5_TO_FLOAT(SFP_10_5_ONE*-0.25) == -0.25f);
    JT_ASSERT(SFP_10_5_TO_FLOAT(FLOAT_TO_SFP_10_5(-0.25f)) == -0.25f);
    
    
    
    // Check whether a number that doesn't round well is represented correctly.
    JT_ASSERT(SFP_10_5_TO_FLOAT(SFP_10_5_ONE*0.3f) == 0.3f);
    
    JT_ASSERT( SFP_10_5_DBG_NEAR_EQUAL(
        SFP_10_5_TO_FLOAT(FLOAT_TO_SFP_10_5(0.3f)), 0.3f, 1
     ) );
    



}

JT_TEST("Multiplication checks"){
    // One should stay one 
    JT_ASSERT( SFP_10_5_MULT(SFP_10_5_ONE, SFP_10_5_ONE, 0, 0) == SFP_10_5_ONE);
    // Basic check, order shouldn't matter, no rounding involved.
    SFP_10_5 four  = FLOAT_TO_SFP_10_5(4);
    SFP_10_5 quarter  = FLOAT_TO_SFP_10_5(0.25f);
    JT_ASSERT(
        SFP_10_5_MULT( quarter, four, 0, 0) == SFP_10_5_ONE
    );
    JT_ASSERT(
        SFP_10_5_MULT( four, quarter, 0, 0) == SFP_10_5_ONE
    );
    // Check the epsilon behaviour, for all x_preshifts
    SFP_10_5 eps = FLOAT_TO_SFP_10_5(SFP_10_5_EPSILON);
    JT_ASSERT(
        SFP_10_5_MULT( SFP_10_5_ONE, eps, 0, 0) == eps
    );
    JT_ASSERT(
        SFP_10_5_MULT( SFP_10_5_ONE, eps, 1, 0) == eps
    );
    JT_ASSERT(
        SFP_10_5_MULT( SFP_10_5_ONE, eps, 2, 0) == eps
    );
    JT_ASSERT(
        SFP_10_5_MULT( SFP_10_5_ONE, eps, 3, 0) == eps
    );
    JT_ASSERT(
        SFP_10_5_MULT( SFP_10_5_ONE, eps, 4, 0) == eps
    );
}

}