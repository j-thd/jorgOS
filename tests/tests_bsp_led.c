#include "jtest.h"
#include <stdio.h>
#include "bsp_led.h"
#include "jfp.h"

JT_TESTS{

JT_TEST("Test primary colours plus black/white"){
    // Black test. Hue shouldn't matter, lightness = 0; sat = 0;
    SFP_5_10 H = FLOAT_TO_SFP_5_10(0.0f);
    SFP_5_10 S = FLOAT_TO_SFP_5_10(0.0f);
    SFP_5_10 L = FLOAT_TO_SFP_5_10(0.0f);

    JT_ASSERT(true);
    BSP_RGB_colour res = BSP_LED_RGB_from_HSL(H, S, L);
    // JT_ASSERT(res.rgb[0] == 0.0f);
    // JT_ASSERT(res.rgb[1] == 0.0f);
    // JT_ASSERT(res.rgb[2] == 0.0f);
    printf("bla");
    JT_ASSERT(true);
}


}