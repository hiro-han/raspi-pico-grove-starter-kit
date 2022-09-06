
set(APP usb_serial_app)

file(GLOB SOURCE *.cpp) 

add_executable(${APP} ${SOURCE})

pico_add_extra_outputs(${APP})

pico_enable_stdio_usb(${APP} 1)
pico_enable_stdio_uart(${APP} 0)

target_link_libraries(${APP} grove-sensors pico_stdlib pico_stdio_usb hardware_i2c)
target_include_directories(${APP} PUBLIC ${GROVE_LIB_INCLUDE})
