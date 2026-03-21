cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED ENV{PS4_PAYLOAD_SDK})
    message(FATAL_ERROR "PS4_PAYLOAD_SDK is undefined")
endif()

project(hello_cmake C CXX)
add_executable(hello_c main.c)
add_executable(hello_cpp main.cpp)

set_target_properties(hello_c PROPERTIES OUTPUT_NAME hello_c.elf)
set_target_properties(hello_cpp PROPERTIES OUTPUT_NAME hello_cpp.elf)

add_custom_target(test_c
    COMMAND ${PS4_DEPLOY} hello_c.elf
    DEPENDS hello_c
)

add_custom_target(test_cpp
    COMMAND ${PS4_DEPLOY} hello_cpp.elf
    DEPENDS hello_cpp
)
