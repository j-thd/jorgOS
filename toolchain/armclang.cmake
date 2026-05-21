# Toolchain file for armclang

# Generic is for bare-metal embedded systems
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# Make sure that the test compilation does not try to generate and run an executable.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH "C:/Program Files/ArmCompilerforEmbedded6.22/bin")

# Compilers to use in this toolchain
set(CMAKE_C_COMPILER "C:/Program Files/ArmCompilerforEmbedded6.22/bin/armclang.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/ArmCompilerforEmbedded6.22/bin/armclang.exe")
set(CMAKE_ASM_COMPILER "C:/Program Files/ArmCompilerforEmbedded6.22/bin/armclang.exe")

# Set tools for making the binary with fromelf
set(FROMELF_EXE "C:/Program Files/ArmCompilerforEmbedded6.22/bin/fromelf.exe")

