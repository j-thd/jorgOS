#include "jfp.h"
#include "jtest.h"
#include <stdio.h>

JT_TESTS{

JT_TEST("Assert test"){

    JT_ASSERT(2 == 2);
    JT_ASSERT(2 == 2);

    //JT_TEST_END;
}
JT_TEST("MOAR TESTS!"){
    JT_ASSERT(1);
    JT_ASSERT(1);
    JT_ASSERT(1);
    JT_ASSERT(1);
    JT_ASSERT(1);
    JT_ASSERT(1);

    //JT_TEST_END;

}

}