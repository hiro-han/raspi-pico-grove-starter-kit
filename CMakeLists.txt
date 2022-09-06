cmake_minimum_required(VERSION 3.12)


# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(grove-sensors-project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(GROVE_LIB_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Initialize the SDK
pico_sdk_init()

# add_compile_options(-Wall
#         -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
#         -Wno-unused-function # we have some for the docs that aren't called
#         -Wno-maybe-uninitialized
#         )

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tool)
