target_link_libraries(bsp targetDeviceLib runtime_environment jorgOS jlib)
target_sources(bsp
    PRIVATE
        bsp.c
        bsp.h
        bsp_gpio.c
        bsp_gpio.h
        bsp_led.c
        bsp_led.h
        bsp_switch.c
        bsp_switch.h
        bsp_timer.c
        bsp_timer.h
        qei.c
        qei.h
        uart.c
        uart.h

    PUBLIC
        FILE_SET bspHeaders
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            bsp.h
            bsp_gpio.h
            bsp_led.h
            bsp_switch.h
            bsp_timer.h
            qei.h
            uart.h
)