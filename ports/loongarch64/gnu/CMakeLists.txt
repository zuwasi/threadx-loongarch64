# Eclipse ThreadX LoongArch64/GNU port
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT

set(TX_LA64_PORT_SOURCES
)

target_sources(${PROJECT_NAME} PRIVATE ${TX_LA64_PORT_SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/inc
)
