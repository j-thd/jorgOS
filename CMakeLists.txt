cmake_minimum_required(VERSION 3.29.0)

project(jorgOS)

# Enable assembly for at least the startup file.
enable_language(ASM)

# Target configuration
set(TARGET_DEVICE TM4C123GH6PM)
set(TARGET_CPU cortex-m4)
set(STARTUP_CODE ${CMAKE_CURRENT_SOURCE_DIR}/device/${TARGET_DEVICE})
set(TARGET_MAP_FILE ${TARGET_DEVICE}_map_file.txt)
set(TARGET_SCATTER_FILE ${STARTUP_CODE}/scatter.txt)

add_executable(main)

# Set the compile options for the specific mcu

set(ARM_FLAGS "-mcpu=${TARGET_CPU} -mthumb -mfpu=none -mfloat-abi=soft --target=arm-arm-none-eabi")
set(COMPILER_FLAGS "-c -g -std=c11")
set(LINKER_FLAGS "--info sizes,stack --map --list target-linking-info.txt --library_type=microlib --scatter=${TARGET_SCATTER_FILE}")
# Make sure a change in the linker script rebuilds the project
set_target_properties(main PROPERTIES LINK_DEPENDS ${TARGET_SCATTER_FILE})

# I do not yet know why the ARM-flags do not have the desired effect with add_compile_options(). I suppose add_compile_options does not always pass arguments when I expect them too.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ARM_FLAGS} ${COMPILER_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${ARM_FLAGS} -masm=auto -c -g -std=c11")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_FLAGS}")

# Define CMSIS include directories
set(CMSIS_INCLUDE_PATH C:/code/custom-cmsis)
set(CMSIS_CORE_INCLUDE_PATH C:/code/custom-cmsis/CMSIS-arm-default/Core/Include)
add_compile_definitions(-D${TARGET_DEVICE} -D__MICROLIB)

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
add_custom_command(TARGET main POST_BUILD COMMAND echo Post build steps...)
add_custom_command(TARGET main POST_BUILD COMMAND echo Creating image info
        COMMAND ${FROMELF_EXE} main.elf --text -s -o image_info.txt)
add_custom_command(TARGET main POST_BUILD COMMAND echo Creating binary image
        COMMAND ${FROMELF_EXE} main.elf --bin -o main.bin)
add_custom_command(TARGET main POST_BUILD COMMAND echo Flashing binary image
        COMMAND LMFLASH main.bin -r -v -i ICDI)
#add_custom_command(TARGET main POST_BUILD COMMAND ${CMAKE_OBJCOPY}  main ${TARGET_DEVICE}.bin )

