
set(GROVE_LIB grove-sensors)

file(GLOB SOURCE *.cpp) 

add_library(${GROVE_LIB} STATIC ${SOURCE})

target_link_libraries(${GROVE_LIB} pico_stdlib hardware_i2c)
