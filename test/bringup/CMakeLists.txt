# M2 bring-up tests
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT

la64_add_qemu_executable(bringup_test bringup_main.c bringup_trap.S)
la64_add_qemu_executable(bringup_fatal_test bringup_fatal.c bringup_break.S)
la64_add_qemu_executable(bringup_memory_test bringup_memory.c)
target_compile_options(bringup_memory_test PRIVATE
    -fno-builtin-memcpy -fno-builtin-memmove
    -fno-builtin-memset -fno-builtin-memcmp)
la64_add_qemu_executable(bringup_long_idle_test bringup_long_idle.c)
la64_add_qemu_executable(bringup_relinquish_test bringup_relinquish.c)
la64_add_qemu_executable(bringup_uart_rx_test bringup_uart_rx.c)

add_test(NAME bringup_timer_and_posture
         COMMAND ${LA64_RUN_QEMU} $<TARGET_FILE:bringup_test>)

add_test(NAME bringup_fatal_exception
         COMMAND ${LA64_RUN_QEMU} --expect "Ecode=0x000000000000000c"
                 $<TARGET_FILE:bringup_fatal_test>)

add_test(NAME bringup_memory_functions
         COMMAND ${LA64_RUN_QEMU} $<TARGET_FILE:bringup_memory_test>)
add_test(NAME bringup_long_idle
         COMMAND ${LA64_RUN_QEMU} --timeout 10 $<TARGET_FILE:bringup_long_idle_test>)
add_test(NAME bringup_relinquish_interrupts_enabled
         COMMAND ${LA64_RUN_QEMU} --timeout 30 $<TARGET_FILE:bringup_relinquish_test>)
set_tests_properties(bringup_long_idle PROPERTIES TIMEOUT 20)
set_tests_properties(bringup_relinquish_interrupts_enabled PROPERTIES TIMEOUT 40)

add_test(NAME bringup_uart_rx_interrupts
         COMMAND ${LA64_RUN_QEMU}
                 --rx-file ${CMAKE_CURRENT_SOURCE_DIR}/uart_rx_payload.txt
                 --rx-after "RX READY" --timeout 30
                 $<TARGET_FILE:bringup_uart_rx_test>)
set_tests_properties(bringup_uart_rx_interrupts PROPERTIES TIMEOUT 40)
