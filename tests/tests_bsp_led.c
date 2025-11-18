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

    // Yellow
    H = FLOAT_TO_SFP_11_20(60.0f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 255);
    JT_ASSERT(res.rgb[1] >= 254 && res.rgb[1] <= 255);
    JT_ASSERT(res.rgb[2] == 0);

    // Green
    H = FLOAT_TO_SFP_11_20(120.0f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 0);
    JT_ASSERT(res.rgb[1] == 255);
    JT_ASSERT(res.rgb[2] == 0);

    // Cyan
    H = FLOAT_TO_SFP_11_20(180.0f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 0);
    JT_ASSERT(res.rgb[1] == 255);
    JT_ASSERT(res.rgb[2] >= 254 && res.rgb[2] <= 255);

    // Blue
    H = FLOAT_TO_SFP_11_20(240.0f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] == 0);
    JT_ASSERT(res.rgb[1] == 0);
    JT_ASSERT(res.rgb[2] == 255);

    // Magenta
    H = FLOAT_TO_SFP_11_20(300.0f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    JT_ASSERT(res.rgb[0] >= 254 && res.rgb[0] <= 255);
    JT_ASSERT(res.rgb[1] == 0);
    JT_ASSERT(res.rgb[2] == 255);
}

JT_TEST("Semi-arbitrary colour test"){
    // Within each 60 degree section, test 1 colours with an arbitrary H,S,L
    // value.
    // In practice this means just picking things from the swatches on
    // wikipedia:
    // https://en.wikipedia.org/wiki/HSL_and_HSV#Swatches
    // 1 out of 255 accuracy is fine.
    


    // 30 degrees.
    SFP_11_20 H = FLOAT_TO_SFP_11_20(30.0f);
    SFP_11_20 S = FLOAT_TO_SFP_11_20(0.75f);
    SFP_11_20 L = FLOAT_TO_SFP_11_20(0.875f);
    BSP_RGB_colour res = BSP_LED_RGB_from_HSL(H, S, L);
    // RGB = F7DFC7
    JT_ASSERT(res.rgb[0] >= 0xF6 && res.rgb[0] <= 0xF8);
    JT_ASSERT(res.rgb[1] >= 0xDE && res.rgb[1] <= 0xE0);
    JT_ASSERT(res.rgb[2] >= 0xC6 && res.rgb[2] <= 0xC8);

    // 90 degrees.
    H = FLOAT_TO_SFP_11_20(90.0f);
    S = FLOAT_TO_SFP_11_20(0.25f);
    L = FLOAT_TO_SFP_11_20(0.125f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    // RGB = 202818
    JT_ASSERT(res.rgb[0] >= 0x19 && res.rgb[0] <= 0x21);
    JT_ASSERT(res.rgb[1] >= 0x27 && res.rgb[1] <= 0x29);
    JT_ASSERT(res.rgb[2] >= 0x17 && res.rgb[2] <= 0x19);

    // 150 degrees.
    H = FLOAT_TO_SFP_11_20(150.0f);
    S = FLOAT_TO_SFP_11_20(1.0f);
    L = FLOAT_TO_SFP_11_20(0.625f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    // RGB = 40FF9F
    JT_ASSERT(res.rgb[0] >= 0x39 && res.rgb[0] <= 0x41);
    JT_ASSERT(res.rgb[1] >= 0xFE && res.rgb[1] <= 0xFF);
    JT_ASSERT(res.rgb[2] >= 0x9E && res.rgb[2] <= 0xA0);

    // 210 degrees.
    H = FLOAT_TO_SFP_11_20(210.0f);
    S = FLOAT_TO_SFP_11_20(0.5f);
    L = FLOAT_TO_SFP_11_20(0.875f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    // RGB = CFDFEF
    JT_ASSERT(res.rgb[0] >= 0xCE && res.rgb[0] <= 0xD0);
    JT_ASSERT(res.rgb[1] >= 0xDE && res.rgb[1] <= 0xE0);
    JT_ASSERT(res.rgb[2] >= 0xEE && res.rgb[2] <= 0xF0);

    // 270 degrees.
    H = FLOAT_TO_SFP_11_20(270.0f);
    S = FLOAT_TO_SFP_11_20(0.25f);
    L = FLOAT_TO_SFP_11_20(0.375f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    // RGB = 604878
    JT_ASSERT(res.rgb[0] >= 0x59 && res.rgb[0] <= 0x61);
    JT_ASSERT(res.rgb[1] >= 0x47 && res.rgb[1] <= 0x49);
    JT_ASSERT(res.rgb[2] >= 0x77 && res.rgb[2] <= 0x79);

    // 330 degrees.
    H = FLOAT_TO_SFP_11_20(330.0f);
    S = FLOAT_TO_SFP_11_20(1.0f);
    L = FLOAT_TO_SFP_11_20(0.375f);
    res = BSP_LED_RGB_from_HSL(H, S, L);
    // RGB = BF0060
    JT_ASSERT(res.rgb[0] >= 0xBE && res.rgb[0] <= 0xC0);
    JT_ASSERT(res.rgb[1] >= 0x00 && res.rgb[1] <= 0x01);
    JT_ASSERT(res.rgb[2] >= 0x59 && res.rgb[2] <= 0x61);
}

}