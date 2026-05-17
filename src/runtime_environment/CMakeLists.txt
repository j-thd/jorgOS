# Include CMSIS and define the device
target_include_directories(runtime_environment INTERFACE ${CMSIS_INCLUDE_PATH} ${CMSIS_CORE_INCLUDE_PATH})

target_include_directories(runtime_environment INTERFACE .)
target_sources(runtime_environment
    PUBLIC
        FILE_SET runtimeEnvironmentHeaders
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            runtime_environment.h
)
