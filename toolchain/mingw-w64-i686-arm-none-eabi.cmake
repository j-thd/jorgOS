# Toolchain file for gcc-arm provided by ARM

# Generic is for bare-metal embedded systems
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Compilers to use in this toolchain
set(CMAKE_C_COMPILER arm-none-eabi-gcc.exe)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# Override test compile target type that doesn't work when cross-compiling

set(CMAKE_FIND_ROOT_PATH C:/tools/arm-compiler/GNU-mingw-w64-i686-arm-none-eabi/bin)