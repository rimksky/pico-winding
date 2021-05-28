cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(winding_project)

pico_sdk_init()

add_executable(winding winding.c)

# Pull in our (to be renamed) simple get you started dependencies
target_link_libraries(winding pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(winding)

#pico_enable_stdio_usb(winding 1)
#pico_enable_stdio_uart(winding 1)