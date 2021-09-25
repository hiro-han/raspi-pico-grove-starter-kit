
set(APP onboard_led)

file(GLOB SOURCE *.cpp) 

add_executable(${APP} ${SOURCE})

pico_add_extra_outputs(${APP})

target_link_libraries(${APP} pico_stdlib)
