# Eclipse ThreadX for LoongArch64
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/loongarch64_gnu.cmake)
endif()

project(threadx_loongarch64 LANGUAGES C ASM)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(THREADX_CUSTOM_PORT ${CMAKE_CURRENT_LIST_DIR}/ports/loongarch64/gnu)

add_subdirectory(external/threadx)
