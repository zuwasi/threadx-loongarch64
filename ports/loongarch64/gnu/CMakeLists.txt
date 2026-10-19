# Eclipse ThreadX LoongArch64/GNU port
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT

set(TX_LA64_PORT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/tx_thread_interrupt_control.S
    ${CMAKE_CURRENT_LIST_DIR}/libc/src/tx_la64_string.c
)

target_sources(${PROJECT_NAME} PRIVATE ${TX_LA64_PORT_SOURCES})

# Keep GCC from replacing the byte loops with calls to the functions being
# defined.
set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/libc/src/tx_la64_string.c
    TARGET_DIRECTORY ${PROJECT_NAME}
    PROPERTIES COMPILE_OPTIONS "-fno-tree-loop-distribute-patterns"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/inc
    ${CMAKE_CURRENT_LIST_DIR}/libc/include
)
