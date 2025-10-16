#include "jfp.h"
#include "jtest.h"
#include <stdio.h>
#include <stdint.h>

JT_TESTS{

JT_TEST("Float Conversion checks"){
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
    
    JT_ASSERT(SFP_10_5_TO_FLOAT(FLOAT_TO_SFP_10_5(16.0f-0.00001f)) == 16.0f);

    // Check whether a number that doesn't round well is represented correctly.
    JT_ASSERT(SFP_10_5_TO_FLOAT(SFP_10_5_ONE*0.3f) == 0.3f);
    
    JT_ASSERT( SFP_10_5_DBG_NEAR_EQUAL(
        SFP_10_5_TO_FLOAT(FLOAT_TO_SFP_10_5(0.3f)), 0.3f, 1
     ) );
    



}

JT_TEST("Int conversion checks"){
    // The same behavior as (int)float_value is is expected.
    
    // +512 is the limit of the SFP.
    
    // printf("\n%X\n", (signed int)x4);
    float x5 = 16.0f - 0.00001f;
    // printf("\nx5 = %f", x5);
    // printf("\n%X", FLOAT_TO_SFP_10_5(x5));
    // printf("\n%X", SFP_10_5_TO_INT(FLOAT_TO_SFP_10_5(x5)));
    // printf("\n%X\n", (signed int)x5);
    float x6 = 16.0f - 0.000001f;
    // printf("\nx6 = %f", x6);
    // printf("\n%X", FLOAT_TO_SFP_10_5(x6));
    // printf("\n%X", SFP_10_5_TO_INT(FLOAT_TO_SFP_10_5(x6)));
    // printf("\n%X\n", (signed int)x6);
    float x7 = 16.0f - 0.0000001f;
    // printf("\nx7 = %f", x7);
    // printf("\n%X", FLOAT_TO_SFP_10_5(x7));
    // printf("\n%X", SFP_10_5_TO_INT(FLOAT_TO_SFP_10_5(x7)));
    // printf("\n%X\n", (signed int)x7);
    SFP_10_5 a = FLOAT_TO_SFP_10_5(16.0f-SFP_10_5_EPSILON);
    SFP_10_5 a_2 = FLOAT_TO_SFP_10_5(x6);
    SFP_10_5 b = FLOAT_TO_SFP_10_5(x7);
    SFP_10_5 b_2 = FLOAT_TO_SFP_10_5(16.0f - 0.00001f);
    SFP_10_5 b_3 = FLOAT_TO_SFP_10_5(x5);
    SFP_10_5 c = FLOAT_TO_SFP_10_5(511.7);
    SFP_10_5 d = FLOAT_TO_SFP_10_5(-16);
    SFP_10_5 e = FLOAT_TO_SFP_10_5(-512);
    SFP_10_5 f = FLOAT_TO_SFP_10_5(0.0f);
    JT_ASSERT((int16_t)((1<<5)*x7) == (int16_t)((1<<5)* (16.0f - 0.0000001f)));
    // Don't really understand why it seems to matter whether a float is defined
    // first with something like x5 = 16.0f-0.00001f.
    // Tests for casting back to int differ if:
    // SFP_10_5 b_2 = FLOAT_TO_SFP_10_5(16.0f - 0.00001f);
    //  SFP_10_5 b_3 = FLOAT_TO_SFP_10_5(x5); 
    // I think compile-time float calculations might be weird.
    JT_ASSERT(SFP_10_5_TO_INT(a) == 15);
    JT_ASSERT(SFP_10_5_TO_INT(a_2) == 15);
    JT_ASSERT(SFP_10_5_TO_INT(b) == 16);
    JT_ASSERT(SFP_10_5_TO_INT(b_2) == 16);
    JT_ASSERT(SFP_10_5_TO_INT(b_3) == 15); 
    JT_ASSERT(SFP_10_5_TO_INT(c) == 511);
    JT_ASSERT(SFP_10_5_TO_INT(d) == -16);
    JT_ASSERT(SFP_10_5_TO_INT(e) == -512);
    JT_ASSERT(SFP_10_5_TO_INT(f) == 0);
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
    // Check the epsilon behaviour, for all x_preshifts and y_preshifts
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
    JT_ASSERT(
        SFP_10_5_MULT( eps, SFP_10_5_ONE, 0, 0) == eps
    );
    JT_ASSERT(
        SFP_10_5_MULT( eps, SFP_10_5_ONE, 0, 1) == eps
    );
    JT_ASSERT(
        SFP_10_5_MULT( eps, SFP_10_5_ONE, 0, 2) == eps
    );
    JT_ASSERT(
        SFP_10_5_MULT( eps, SFP_10_5_ONE, 0, 3) == eps
    );
    JT_ASSERT(
        SFP_10_5_MULT( eps, SFP_10_5_ONE, 0, 4) == eps
    );
    JT_ASSERT(
        SFP_10_5_MULT( eps, SFP_10_5_ONE, 0, 5) == eps
    );

    // +512 is the limit of the SFP.
    SFP_10_5 a  = FLOAT_TO_SFP_10_5(16);
    SFP_10_5 b  = FLOAT_TO_SFP_10_5(32);
    SFP_10_5 c = FLOAT_TO_SFP_10_5(512);
    SFP_10_5 d  = FLOAT_TO_SFP_10_5(-16);
    SFP_10_5 e = FLOAT_TO_SFP_10_5(-512);
    SFP_10_5 f = FLOAT_TO_SFP_10_5(256);
    
    JT_ASSERT(
        SFP_10_5_MULT( a, b, 0, 0) == c
    );
    JT_ASSERT(
        SFP_10_5_MULT( a, b, 2, 3) == c
    );

    JT_ASSERT(
        SFP_10_5_MULT( d, b, 0, 0) == e
    );
    JT_ASSERT(
        SFP_10_5_MULT(d, d, 0, 0) == f
    );


}

JT_TEST("Modulo 2 / remainder checks"){
    JT_ASSERT(SFP_10_5_ONE == SFP_10_5_MOD_2(SFP_10_5_ONE));

    // Check zero, one, minus one inputs
    SFP_10_5 zero  = FLOAT_TO_SFP_10_5(0);
    JT_ASSERT(SFP_10_5_MOD_2(zero) == zero);

    SFP_10_5 one  = FLOAT_TO_SFP_10_5(1);
    JT_ASSERT(SFP_10_5_MOD_2(one) == one);

    SFP_10_5 minus_one  = FLOAT_TO_SFP_10_5(-1);
    JT_ASSERT(SFP_10_5_MOD_2(minus_one) == minus_one);
    
    // Basic checks with integers, and negative values.
    SFP_10_5 a  = FLOAT_TO_SFP_10_5(16);
    SFP_10_5 b = FLOAT_TO_SFP_10_5(0);
    JT_ASSERT(SFP_10_5_MOD_2(a) == b);

    SFP_10_5 c  = FLOAT_TO_SFP_10_5(17);
    SFP_10_5 d = FLOAT_TO_SFP_10_5(1);
    JT_ASSERT(SFP_10_5_MOD_2(c) == d);

    SFP_10_5 e  = FLOAT_TO_SFP_10_5(-32);
    SFP_10_5 f = FLOAT_TO_SFP_10_5(0);
    JT_ASSERT(SFP_10_5_MOD_2(e) == f);

    SFP_10_5 g  = FLOAT_TO_SFP_10_5(-15);
    SFP_10_5 h = FLOAT_TO_SFP_10_5(-1);
    JT_ASSERT(SFP_10_5_MOD_2(g) == h);


    // Checking floats
    SFP_10_5 i = FLOAT_TO_SFP_10_5(0.5f);
    JT_ASSERT( SFP_10_5_MOD_2(i) == (SFP_10_5_ONE*0.5f)) ;

    SFP_10_5 j = FLOAT_TO_SFP_10_5(-32.125f);
    JT_ASSERT( SFP_10_5_MOD_2(j) == (SFP_10_5_ONE*-0.125f)) ;

    // Checking upper and lower bounds
    SFP_10_5 k = FLOAT_TO_SFP_10_5(512);
    JT_ASSERT( SFP_10_5_MOD_2(k) == 0) ;

    SFP_10_5 l = FLOAT_TO_SFP_10_5(-512);
    JT_ASSERT( SFP_10_5_MOD_2(l) == 0) ;
}

}