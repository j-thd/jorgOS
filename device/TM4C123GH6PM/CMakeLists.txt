target_link_libraries(targetDeviceLib runtime_environment)
target_sources(targetDeviceLib
    PRIVATE
        system_TM4C123.c
        startup_TM4C123.s
)