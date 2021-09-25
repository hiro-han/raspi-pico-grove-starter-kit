
set(APP temperature_humidity)

file(GLOB SOURCE *.cpp) 

add_executable(${APP} ${SOURCE})

pico_add_extra_outputs(${APP})

target_link_libraries(${APP} grove-sensors pico_stdlib hardware_i2c)
target_include_directories(${APP} PUBLIC ${GROVE_LIB_INCLUDE})
