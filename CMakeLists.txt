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

enable_testing()

# Warnings for code owned by this project (port, BSP, tests). ThreadX common
# code keeps its own settings.
set(LA64_STRICT_WARNINGS -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -Werror)
set(LA64_RUN_QEMU bash ${CMAKE_CURRENT_LIST_DIR}/tools/run_qemu.sh)

set(THREADX_CUSTOM_PORT ${CMAKE_CURRENT_LIST_DIR}/ports/loongarch64/gnu)
add_subdirectory(external/threadx)

add_subdirectory(bsp/qemu_virt)
add_subdirectory(test/bringup)
add_subdirectory(test/threadx_demo)
