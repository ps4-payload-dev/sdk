#   Copyright (C) 2026 John Törnblom
#
# This file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING. If not see
# <http://www.gnu.org/licenses/>.

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
