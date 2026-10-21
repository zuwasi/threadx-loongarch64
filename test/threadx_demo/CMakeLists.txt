# ThreadX scheduler, timer, and context-switch smoke test.
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT

la64_add_qemu_executable(threadx_demo demo_main.c)
add_test(
    NAME threadx_demo
    COMMAND ${LA64_RUN_QEMU} --timeout 60 $<TARGET_FILE:threadx_demo>
)
set_tests_properties(threadx_demo PROPERTIES TIMEOUT 75)
