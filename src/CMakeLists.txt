target_sources(main PRIVATE main.c)

add_subdirectory(runtime_environment)

add_subdirectory(jorgOS)
add_subdirectory(jlib)
add_subdirectory(features)
add_subdirectory(bsp)