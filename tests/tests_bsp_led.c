#include "jtest.h"
#include <stdio.h>
#include "bsp_led.h"
#include "jfp.h"

JT_TESTS{

JT_TEST("Test black"){
    // Black test. Hue shouldn't matter, lightness = 0; sat = 0;
    // Cycle through the six cases that for hue dash that are between [0,60,120,180,240,360)
    
    BSP_LED_RGB rgb;
    BSP_LED_HSL hsl = {
        0,
        0,
        0
    };
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 0);
    JT_ASSERT(rgb[1] == 0);
    JT_ASSERT(rgb[2] == 0);

    // Black 60-120
    hsl[0] = FLOAT_TO_SFP_11_20(70.0f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 0);
    JT_ASSERT(rgb[1] == 0);
    JT_ASSERT(rgb[2] == 0);

    // Black 120-180
    hsl[0] = FLOAT_TO_SFP_11_20(171.0f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 0);
    JT_ASSERT(rgb[1] == 0);
    JT_ASSERT(rgb[2] == 0);

    // Black 180-240
    hsl[0] = FLOAT_TO_SFP_11_20(193.123f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 0);
    JT_ASSERT(rgb[1] == 0);
    JT_ASSERT(rgb[2] == 0);

    // Saturation shouldn't matter for white and black so let's override it and see if shit breaks.
    hsl[1] = FLOAT_TO_SFP_11_20(1.0f);

    // Black 240-300
    hsl[0] = FLOAT_TO_SFP_11_20(280.0f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 0);
    JT_ASSERT(rgb[1] == 0);
    JT_ASSERT(rgb[2] == 0);

    hsl[1] = FLOAT_TO_SFP_11_20(0.56f);
    // Black 300-360
    hsl[0] = FLOAT_TO_SFP_11_20(359.99999999999999f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 0);
    JT_ASSERT(rgb[1] == 0);
    JT_ASSERT(rgb[2] == 0);
}

JT_TEST("Test white"){
    // White test. Hue shouldn't matter, lightness = 1; sat = 0;
    // Cycle through the six cases that for hue dash that are between [0,60,120,180,240,360)
    
    BSP_LED_RGB rgb;
    BSP_LED_HSL hsl = {
            FLOAT_TO_SFP_11_20(0.0f),
            FLOAT_TO_SFP_11_20(0.0f),
            FLOAT_TO_SFP_11_20(1.0f)
        };


    // White 0-60
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 255);
    JT_ASSERT(rgb[1] == 255);
    JT_ASSERT(rgb[2] == 255);

    // White 60-120
    hsl[0] = FLOAT_TO_SFP_11_20(70.0f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 255);
    JT_ASSERT(rgb[1] == 255);
    JT_ASSERT(rgb[2] == 255);

    // White 120-180
    hsl[0] = FLOAT_TO_SFP_11_20(171.0f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 255);
    JT_ASSERT(rgb[1] == 255);
    JT_ASSERT(rgb[2] == 255);

    // Saturation shouldn't matter for white and black so let's override it and see if shit breaks.
    hsl[1] = FLOAT_TO_SFP_11_20(0.23f);

    // White 180-240
    hsl[0] = FLOAT_TO_SFP_11_20(193.123f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 255);
    JT_ASSERT(rgb[1] == 255);
    JT_ASSERT(rgb[2] == 255);

    // White 240-300
    hsl[0] = FLOAT_TO_SFP_11_20(240.0f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 255);
    JT_ASSERT(rgb[1] == 255);
    JT_ASSERT(rgb[2] == 255);

    // White 300-360
    hsl[0] = FLOAT_TO_SFP_11_20(359.99999999999999f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 255);
    JT_ASSERT(rgb[1] == 255);
    JT_ASSERT(rgb[2] == 255);
}

JT_TEST("Test the rainbow!"){
    // Primary colours plus those in between. Hue shouldn't matter, lightness = 0.5f; sat = 1.0f;
    // Cycle through the six exact 60 cases for hue dahs.
    
    // Array that always holds the results
    BSP_LED_RGB rgb;
    BSP_LED_HSL hsl = {
            FLOAT_TO_SFP_11_20(0.0f),
            FLOAT_TO_SFP_11_20(1.0f),
            FLOAT_TO_SFP_11_20(0.5f)
        };


    // Red
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 255);
    JT_ASSERT(rgb[1] == 0);
    JT_ASSERT(rgb[2] == 0);

    // Yellow
    hsl[0] = FLOAT_TO_SFP_11_20(60.0f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 255);
    JT_ASSERT(rgb[1] >= 254 && rgb[1] <= 255);
    JT_ASSERT(rgb[2] == 0);

    // Green
    hsl[0] = FLOAT_TO_SFP_11_20(120.0f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 0);
    JT_ASSERT(rgb[1] == 255);
    JT_ASSERT(rgb[2] == 0);

    // Cyan
    hsl[0] = FLOAT_TO_SFP_11_20(180.0f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 0);
    JT_ASSERT(rgb[1] == 255);
    JT_ASSERT(rgb[2] >= 254 && rgb[2] <= 255);

    // Blue
    hsl[0] = FLOAT_TO_SFP_11_20(240.0f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] == 0);
    JT_ASSERT(rgb[1] == 0);
    JT_ASSERT(rgb[2] == 255);

    // Magenta
    hsl[0] = FLOAT_TO_SFP_11_20(300.0f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    JT_ASSERT(rgb[0] >= 254 && rgb[0] <= 255);
    JT_ASSERT(rgb[1] == 0);
    JT_ASSERT(rgb[2] == 255);
}

JT_TEST("Semi-arbitrary colour test"){
    // Within each 60 degree section, test 1 colours with an arbitrary H,S,L
    // value.
    // In practice this means just picking things from the swatches on
    // wikipedia:
    // https://en.wikipedia.org/wiki/HSL_and_HSV#Swatches
    // 1 out of 255 accuracy is fine.
    
    BSP_LED_RGB rgb;
    BSP_LED_HSL hsl = {
            FLOAT_TO_SFP_11_20(30.0f),
            FLOAT_TO_SFP_11_20(0.75f),
            FLOAT_TO_SFP_11_20(0.875f)
        };

    // 30 degrees.
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    // RGB = F7DFC7
    JT_ASSERT(rgb[0] >= 0xF6 && rgb[0] <= 0xF8);
    JT_ASSERT(rgb[1] >= 0xDE && rgb[1] <= 0xE0);
    JT_ASSERT(rgb[2] >= 0xC6 && rgb[2] <= 0xC8);

    // 90 degrees.
    hsl[0] = FLOAT_TO_SFP_11_20(90.0f);
    hsl[1] = FLOAT_TO_SFP_11_20(0.25f);
    hsl[2] = FLOAT_TO_SFP_11_20(0.125f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    // RGB = 202818
    JT_ASSERT(rgb[0] >= 0x19 && rgb[0] <= 0x21);
    JT_ASSERT(rgb[1] >= 0x27 && rgb[1] <= 0x29);
    JT_ASSERT(rgb[2] >= 0x17 && rgb[2] <= 0x19);

    // 150 degrees.
    hsl[0] = FLOAT_TO_SFP_11_20(150.0f);
    hsl[1] = FLOAT_TO_SFP_11_20(1.0f);
    hsl[2] = FLOAT_TO_SFP_11_20(0.625f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    // RGB = 40FF9F
    JT_ASSERT(rgb[0] >= 0x39 && rgb[0] <= 0x41);
    JT_ASSERT(rgb[1] >= 0xFE && rgb[1] <= 0xFF);
    JT_ASSERT(rgb[2] >= 0x9E && rgb[2] <= 0xA0);

    // 210 degrees.
    hsl[0] = FLOAT_TO_SFP_11_20(210.0f);
    hsl[1] = FLOAT_TO_SFP_11_20(0.5f);
    hsl[2] = FLOAT_TO_SFP_11_20(0.875f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    // RGB = CFDFEF
    JT_ASSERT(rgb[0] >= 0xCE && rgb[0] <= 0xD0);
    JT_ASSERT(rgb[1] >= 0xDE && rgb[1] <= 0xE0);
    JT_ASSERT(rgb[2] >= 0xEE && rgb[2] <= 0xF0);

    // 270 degrees.
    hsl[0] = FLOAT_TO_SFP_11_20(270.0f);
    hsl[1] = FLOAT_TO_SFP_11_20(0.25f);
    hsl[2] = FLOAT_TO_SFP_11_20(0.375f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    // RGB = 604878
    JT_ASSERT(rgb[0] >= 0x59 && rgb[0] <= 0x61);
    JT_ASSERT(rgb[1] >= 0x47 && rgb[1] <= 0x49);
    JT_ASSERT(rgb[2] >= 0x77 && rgb[2] <= 0x79);

    // 330 degrees.
    hsl[0] = FLOAT_TO_SFP_11_20(330.0f);
    hsl[1] = FLOAT_TO_SFP_11_20(1.0f);
    hsl[2] = FLOAT_TO_SFP_11_20(0.375f);
    BSP_LED_RGB_from_HSL(&rgb, &hsl);
    // RGB = BF0060
    JT_ASSERT(rgb[0] >= 0xBE && rgb[0] <= 0xC0);
    JT_ASSERT(rgb[1] >= 0x00 && rgb[1] <= 0x01);
    JT_ASSERT(rgb[2] >= 0x59 && rgb[2] <= 0x61);
}

}