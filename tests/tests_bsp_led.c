#include "jtest.h"
#include <stdio.h>
#include "bsp_led.h"
#include "jfp.h"

JT_TESTS{

JT_TEST("Test black"){
    // Black test. Hue shouldn't matter, lightness = 0; sat = 0;
    // Cycle through the six cases that for hue dash that are between [0,60,120,180,240,360)
    
    SFP_11_20 S = FLOAT_TO_SFP_11_20(0.0f);
    SFP_11_20 L = FLOAT_TO_SFP_11_20(0.0f);

    // Black 0-60
    SFP_11_20 H = FLOAT_TO_SFP_11_20(0.0f);
    BSP_RGB_colour res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 0);
    JT_ASSERT(res.rgb[1] == 0);
    JT_ASSERT(res.rgb[2] == 0);

    // Black 60-120
    H = FLOAT_TO_SFP_11_20(70.0f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 0);
    JT_ASSERT(res.rgb[1] == 0);
    JT_ASSERT(res.rgb[2] == 0);

    // Black 120-180
    H = FLOAT_TO_SFP_11_20(171.0f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 0);
    JT_ASSERT(res.rgb[1] == 0);
    JT_ASSERT(res.rgb[2] == 0);

    // Black 180-240
    H = FLOAT_TO_SFP_11_20(193.123f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 0);
    JT_ASSERT(res.rgb[1] == 0);
    JT_ASSERT(res.rgb[2] == 0);

    // Saturation shouldn't matter for white and black so let's override it and see if shit breaks.
    S = FLOAT_TO_SFP_11_20(1.0f);

    // Black 240-300
    H = FLOAT_TO_SFP_11_20(280.0f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 0);
    JT_ASSERT(res.rgb[1] == 0);
    JT_ASSERT(res.rgb[2] == 0);

    S = FLOAT_TO_SFP_11_20(0.56f);
    // Black 300-360
    H = FLOAT_TO_SFP_11_20(359.99999999999999f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 0);
    JT_ASSERT(res.rgb[1] == 0);
    JT_ASSERT(res.rgb[2] == 0);
}

JT_TEST("Test white"){
    // White test. Hue shouldn't matter, lightness = 1; sat = 0;
    // Cycle through the six cases that for hue dash that are between [0,60,120,180,240,360)
    
    SFP_11_20 S = FLOAT_TO_SFP_11_20(0.0f);
    SFP_11_20 L = FLOAT_TO_SFP_11_20(1.0f);

    // White 0-60
    SFP_11_20 H = FLOAT_TO_SFP_11_20(0.0f);
    BSP_RGB_colour res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[1] == 255);
    JT_ASSERT(res.rgb[2] == 255);

    // White 60-120
    H = FLOAT_TO_SFP_11_20(70.0f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[1] == 255);
    JT_ASSERT(res.rgb[2] == 255);

    // White 120-180
    H = FLOAT_TO_SFP_11_20(171.0f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[1] == 255);
    JT_ASSERT(res.rgb[2] == 255);

    // Saturation shouldn't matter for white and black so let's override it and see if shit breaks.
    S = FLOAT_TO_SFP_11_20(0.23f);

    // White 180-240
    H = FLOAT_TO_SFP_11_20(193.123f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[1] == 255);
    JT_ASSERT(res.rgb[2] == 255);

    // White 240-300
    H = FLOAT_TO_SFP_11_20(240.0f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[1] == 255);
    JT_ASSERT(res.rgb[2] == 255);

    // White 300-360
    H = FLOAT_TO_SFP_11_20(359.99999999999999f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[1] == 255);
    JT_ASSERT(res.rgb[2] == 255);
}

JT_TEST("Test the rainbow!"){
    // Primary colours plus those in between. Hue shouldn't matter, lightness = 0.5f; sat = 1.0f;
    // Cycle through the six exact 60 cases for hue dahs.
    
    SFP_11_20 S = FLOAT_TO_SFP_11_20(1.0f);
    SFP_11_20 L = FLOAT_TO_SFP_11_20(0.5f);

    // Red
    SFP_11_20 H = FLOAT_TO_SFP_11_20(0.0f);
    BSP_RGB_colour res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[1] == 0);
    JT_ASSERT(res.rgb[2] == 0);

    // Black 60-120
    H = FLOAT_TO_SFP_11_20(7255);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[1] == 255);
    JT_ASSERT(res.rgb[2] == 255);

    // Black 120-180
    H = FLOAT_TO_SFP_11_20(171.0f);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[1] == 255);
    JT_ASSERT(res.rgb[2] == 255);

    // Black 180-240
    H = FLOAT_TO_SFP_11_20(193.123f);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[1] == 255);
    JT_ASSERT(res.rgb[2] == 255);

    // Black 240-360
    H = FLOAT_TO_SFP_11_20(359.99999999999999f);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[1] == 255);
    JT_ASSERT(res.rgb[2] == 255);
}


}