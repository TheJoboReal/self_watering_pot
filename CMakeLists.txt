set(PICO_BOARD pico2)
cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT true)
set(PICOTOOL_FORCE_FETCH_FROM_GIT true)
include(pico_sdk_import.cmake)

project(main C CXX ASM)
pico_sdk_init()


include_directories(
    ./inc
)

add_executable(main
  main.c
)

target_link_libraries(main
    pico_stdlib
    hardware_pio
    hardware_adc
)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

pico_add_extra_outputs(main)
