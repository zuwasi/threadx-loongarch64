# M2 bring-up tests
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT

la64_add_qemu_executable(bringup_test bringup_main.c bringup_trap.S)
la64_add_qemu_executable(bringup_fatal_test bringup_fatal.c bringup_break.S)

add_test(NAME bringup_timer_and_posture
         COMMAND ${LA64_RUN_QEMU} $<TARGET_FILE:bringup_test>)

add_test(NAME bringup_fatal_exception
         COMMAND ${LA64_RUN_QEMU} --expect "Ecode=0x000000000000000c"
                 $<TARGET_FILE:bringup_fatal_test>)
