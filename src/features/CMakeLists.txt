target_link_libraries(features bsp)
target_sources(features
    PRIVATE
        led_manager.c
        led_manager.h

    PUBLIC
        FILE_SET featuresHeaders
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            led_manager.h
)