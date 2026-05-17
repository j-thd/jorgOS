cmake_minimum_required(VERSION 3.29.0)

project(jorgOS)

# Enable assembly for at least the startup file.
enable_language(ASM)

# Target configuration
set(TARGET_DEVICE TM4C123GH6PM)
set(TARGET_CPU cortex-m4)
set(STARTUP_CODE device/${TARGET_DEVICE})
set(TARGET_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/device/${TARGET_DEVICE}/${TARGET_DEVICE}.lds)
set(TARGET_MAP_FILE ${TARGET_DEVICE}_map_file.txt)

add_executable(main)

# Set the compile options for the specific mcu

set(ARM_FLAGS "-mcpu=${TARGET_CPU}+nofp -march=armv7e-m -mthumb -mfloat-abi=soft")
set(COMPILER_FLAGS "-fcallgraph-info -eReset_Handler")
set(LINKER_FLAGS "-T${TARGET_LINKER_SCRIPT} -Wl,-Map=${TARGET_MAP_FILE} -specs=nano.specs -specs=nosys.specs " )
# Make sure a change in the linker script rebuilds the project
set_target_properties(main PROPERTIES LINK_DEPENDS ${TARGET_LINKER_SCRIPT})

# I do not yet know why the ARM-flags do not have the desired effect with add_compile_options(). I suppose add_compile_options does not always pass arguments when I expect them too.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ARM_FLAGS} ${COMPILER_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${ARM_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_FLAGS}")

# Define CMSIS include directories
set(CMSIS_INCLUDE_PATH C:/code/custom-cmsis)
set(CMSIS_CORE_INCLUDE_PATH C:/code/custom-cmsis/CMSIS-arm-default/Core/Include)
add_compile_definitions(-D${TARGET_DEVICE})

# Enable JSerialMonitor
add_compile_definitions(-DJSM_ENABLE)

# Adding the device library
add_library(targetDeviceLib)
add_subdirectory(device) # This is where the device (startup) code resides.

# Adding the header-only runtime environment
add_library(runtime_environment INTERFACE)

# Actual libraries with code
add_library(jorgOS)
add_library(jlib)
add_library(features)
add_library(bsp)

add_subdirectory(src)

# Library target sources are in the hierarchy starting at src.
target_link_libraries(main PUBLIC targetDeviceLib runtime_environment jorgOS jlib features bsp )


# Command triggers after building
add_custom_command(TARGET main POST_BUILD COMMAND echo Generating symbol table...)
add_custom_command(TARGET main POST_BUILD COMMAND objdump main -t -h > symtable.txt)

