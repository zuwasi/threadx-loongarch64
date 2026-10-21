# ThreadX upstream regression suite on the LoongArch64 QEMU virt BSP (M4).
#
# Standalone project: configure it separately (tools/regression.sh) so the
# per-configuration kernel defines do not leak into the bring-up and demo
# builds of the root project.
#
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.20)

project(threadx_loongarch64_regression LANGUAGES C ASM)

set(LA64_REPO_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)

# Configurations mirror test/tx/cmake/riscv: default_build, optimized_build,
# disable_notify_callbacks_build, stack_checking_build,
# stack_checking_rand_fill_build, trace_build.
set(LA64_REGRESSION_CONFIGS
    default
    optimized
    disable_notify_callbacks
    stack_checking
    stack_checking_rand_fill
    trace)
set(LA64_REGRESSION_CONFIG "default" CACHE STRING
    "Regression configuration")
set_property(CACHE LA64_REGRESSION_CONFIG PROPERTY STRINGS ${LA64_REGRESSION_CONFIGS})
if(NOT LA64_REGRESSION_CONFIG IN_LIST LA64_REGRESSION_CONFIGS)
    message(FATAL_ERROR
        "Unknown LA64_REGRESSION_CONFIG '${LA64_REGRESSION_CONFIG}'. Valid: ${LA64_REGRESSION_CONFIGS}")
endif()
message(STATUS "Regression configuration: ${LA64_REGRESSION_CONFIG}")

# Per-configuration kernel defines (upstream equivalents in
# test/tx/cmake/riscv/CMakeLists.txt).
set(LA64_CONFIG_DEFINES -DTX_QUEUE_MESSAGE_MAX_SIZE=32)
if(LA64_REGRESSION_CONFIG STREQUAL "disable_notify_callbacks")
    list(APPEND LA64_CONFIG_DEFINES -DTX_DISABLE_NOTIFY_CALLBACKS)
elseif(LA64_REGRESSION_CONFIG STREQUAL "stack_checking")
    list(APPEND LA64_CONFIG_DEFINES -DTX_ENABLE_STACK_CHECKING)
elseif(LA64_REGRESSION_CONFIG STREQUAL "stack_checking_rand_fill")
    list(APPEND LA64_CONFIG_DEFINES
        -DTX_ENABLE_STACK_CHECKING
        -DTX_ENABLE_RANDOM_NUMBER_STACK_FILLING)
elseif(LA64_REGRESSION_CONFIG STREQUAL "trace")
    list(APPEND LA64_CONFIG_DEFINES -DTX_ENABLE_EVENT_TRACE)
endif()

# Harness defines.  CTEST/BATCH_TEST: one test per executable, so
# testcontrol's test_control_tests[] holds only test_application_define.
# EXTERNAL_EXIT: testcontrol reports the result to external_exit().
# TEST_STACK_SIZE_PRINTF: stack size the printf-heavy tests expect.
add_compile_options(
    -DTX_REGRESSION_TEST
    -DCTEST
    -DBATCH_TEST
    -DEXTERNAL_EXIT
    -DTEST_STACK_SIZE_PRINTF=4096
    ${LA64_CONFIG_DEFINES}
)

# The cross compiler targets Linux and therefore predefines __linux__, which
# would compile the POSIX-only paths of some upstream tests (pthread
# injector).  Bare-metal identification matches the upstream harnesses.
add_compile_options(-U__linux__)

# Optimization mirrors the upstream default_build/-O0, optimized_build/-O2
# split and overrides the toolchain's build-type flags.
if(LA64_REGRESSION_CONFIG STREQUAL "optimized")
    add_compile_options(-O2)
else()
    add_compile_options(-O0)
endif()

# Same warning set and QEMU runner the root build uses for project code.
set(LA64_STRICT_WARNINGS -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -Werror)
set(LA64_RUN_QEMU bash ${LA64_REPO_ROOT}/tools/run_qemu.sh)

enable_testing()

# Kernel with the LoongArch64 port, compiled with this configuration's
# defines (directory-level options propagate into the subdirectories).
set(THREADX_CUSTOM_PORT ${LA64_REPO_ROOT}/ports/loongarch64/gnu)
add_subdirectory(${LA64_REPO_ROOT}/external/threadx threadx)

# TX_ENABLE_RANDOM_NUMBER_STACK_FILLING makes tx_thread_create.c call
# TX_RAND(), which defaults to rand() without including <stdlib.h>.  Supply
# the declaration the same way the upstream RISC-V harness does; rand() is
# implemented in libc/src/support.c.
target_compile_options(threadx PRIVATE $<$<COMPILE_LANGUAGE:C>:-include$<SEMICOLON>stdlib.h>)
target_include_directories(threadx SYSTEM PRIVATE ${CMAKE_CURRENT_LIST_DIR}/libc/include)

# QEMU virt BSP: startup, linker script, UART, timer, poweroff.
add_subdirectory(${LA64_REPO_ROOT}/bsp/qemu_virt bsp_qemu_virt)

# Freestanding libc subset the upstream suite needs: <stdio.h>, <stdlib.h>,
# and <time.h> come from libc/include; printf, external_exit, exit, time,
# rand, and srand live in libc/src.
add_library(la64_regression_support
    libc/src/printf.c
    libc/src/support.c
)
target_include_directories(la64_regression_support SYSTEM PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/libc/include
)
target_compile_options(la64_regression_support PRIVATE ${LA64_STRICT_WARNINGS})
target_link_libraries(la64_regression_support PUBLIC bsp_qemu_virt azrtos::threadx)

# testcontrol.c provides main(), tx_application_define(), and the shared
# test-control objects.  Upstream sources call rand()/external_exit()
# without including <stdlib.h>, which gcc-14 rejects as an error, so the
# header is force-included the same way the upstream RISC-V harness does.
set(LA64_TX_TEST_DIR ${LA64_REPO_ROOT}/external/threadx/test)
add_library(la64_test_utility STATIC ${LA64_TX_TEST_DIR}/tx/regression/testcontrol.c)
target_compile_options(la64_test_utility PUBLIC -include stdlib.h)
target_link_libraries(la64_test_utility PUBLIC
    bsp_qemu_virt azrtos::threadx la64_regression_support)

# Weak hook defaults must be objects: the linker skips weak definitions
# during archive scanning, so a static library would drop them.
add_library(la64_test_weak_defaults OBJECT
    ${LA64_TX_TEST_DIR}/shared/regression/testcontrol_weak_defaults.c)

# One bare-metal executable per upstream test, linked against the same BSP
# startup and kernel.
function(la64_add_regression_executable target)
    add_executable(${target} ${ARGN} $<TARGET_OBJECTS:la64_test_weak_defaults>)
    target_link_options(${target} PRIVATE
        -T${BSP_QEMU_VIRT_LINKER_SCRIPT}
        -Wl,--undefined=_start
        -Wl,-Map=$<TARGET_FILE:${target}>.map
    )
    target_link_libraries(${target} PRIVATE
        -Wl,--whole-archive la64_regression_support -Wl,--no-whole-archive
        -Wl,--start-group bsp_qemu_virt threadx la64_test_utility -Wl,--end-group
    )
    set_target_properties(${target} PROPERTIES
        SUFFIX ".elf"
        LINK_DEPENDS ${BSP_QEMU_VIRT_LINKER_SCRIPT}
    )
endfunction()

# Run one test on QEMU.  tools/run_qemu.sh judges the console: external_exit
# prints *** TEST PASSED *** or *** TEST FAILED *** before powering off.
function(la64_register_regression_test target qemu_timeout)
    math(EXPR la64_ctest_timeout "${qemu_timeout} + 30")
    add_test(
        NAME ${LA64_REGRESSION_CONFIG}::${target}
        COMMAND ${LA64_RUN_QEMU} --timeout ${qemu_timeout} $<TARGET_FILE:${target}>
    )
    set_tests_properties(${LA64_REGRESSION_CONFIG}::${target}
        PROPERTIES TIMEOUT ${la64_ctest_timeout})
endfunction()

# Upstream regression cases (test/tx/regression): the same set the upstream
# RISC-V QEMU harness runs, minus its RISC-V-specific FPU test.
set(LA64_REGRESSION_CASES
    threadx_block_memory_basic_test
    threadx_block_memory_error_detection_test
    threadx_block_memory_information_test
    threadx_block_memory_prioritize_test
    threadx_block_memory_suspension_test
    threadx_block_memory_suspension_timeout_test
    threadx_block_memory_thread_terminate_test
    threadx_byte_memory_basic_test
    threadx_byte_memory_information_test
    threadx_byte_memory_prioritize_test
    threadx_byte_memory_suspension_test
    threadx_byte_memory_suspension_timeout_test
    threadx_byte_memory_thread_contention_test
    threadx_byte_memory_thread_terminate_test
    threadx_event_flag_basic_test
    threadx_event_flag_information_test
    threadx_event_flag_isr_set_clear_test
    threadx_event_flag_isr_wait_abort_test
    threadx_event_flag_single_thread_terminate_test
    threadx_event_flag_suspension_consume_test
    threadx_event_flag_suspension_different_bits_consume_test
    threadx_event_flag_suspension_different_bits_test
    threadx_event_flag_suspension_test
    threadx_event_flag_suspension_timeout_test
    threadx_event_flag_thread_terminate_test
    threadx_interrupt_control_test
    threadx_mutex_basic_test
    threadx_mutex_delete_test
    threadx_mutex_information_test
    threadx_mutex_nested_priority_inheritance_test
    threadx_mutex_no_preemption_test
    threadx_mutex_preemption_test
    threadx_mutex_priority_inheritance_test
    threadx_mutex_proritize_test
    threadx_mutex_suspension_timeout_test
    threadx_mutex_thread_terminate_test
    threadx_queue_basic_eight_word_test
    threadx_queue_basic_four_word_test
    threadx_queue_basic_one_word_test
    threadx_queue_basic_sixteen_word_test
    threadx_queue_basic_two_word_test
    threadx_queue_basic_max_message_size_test
    threadx_queue_empty_suspension_test
    threadx_queue_flush_no_suspension_test
    threadx_queue_flush_test
    threadx_queue_front_send_test
    threadx_queue_full_suspension_test
    threadx_queue_information_test
    threadx_queue_prioritize
    threadx_queue_suspension_timeout_test
    threadx_queue_thread_terminate_test
    threadx_semaphore_basic_test
    threadx_semaphore_ceiling_put_test
    threadx_semaphore_delete_test
    threadx_semaphore_information_test
    threadx_semaphore_non_preemption_test
    threadx_semaphore_preemption_test
    threadx_semaphore_prioritize
    threadx_semaphore_thread_terminate_test
    threadx_semaphore_timeout_test
    threadx_thread_basic_execution_test
    threadx_thread_basic_time_slice_test
    threadx_thread_completed_test
    threadx_thread_create_preemption_threshold_test
    threadx_thread_delayed_suspension_test
    threadx_thread_information_test
    threadx_thread_multi_level_preemption_threshold_test
    threadx_thread_multiple_non_current_test
    threadx_thread_multiple_sleep_test
    threadx_thread_multiple_suspension_test
    threadx_thread_multiple_time_slice_test
    threadx_thread_preemptable_suspension_test
    threadx_thread_preemption_change_test
    threadx_thread_priority_change
    threadx_thread_relinquish_test
    threadx_thread_reset_test
    threadx_thread_simple_sleep_non_clear_test
    threadx_thread_simple_sleep_test
    threadx_thread_simple_suspend_test
    threadx_thread_sleep_for_100ticks_test
    threadx_thread_sleep_terminate_test
    threadx_thread_stack_checking_test
    threadx_thread_terminate_delete_test
    threadx_thread_time_slice_change_test
    threadx_thread_wait_abort_and_isr_test
    threadx_thread_wait_abort_test
    threadx_time_get_set_test
    threadx_timer_activate_deactivate_test
    threadx_timer_deactivate_accuracy_test
    threadx_timer_information_test
    threadx_timer_large_timer_accuracy_test
    threadx_timer_multiple_accuracy_test
    threadx_timer_multiple_test
    threadx_timer_simple_test
    threadx_trace_basic_test
)

foreach(case ${LA64_REGRESSION_CASES})
    la64_add_regression_executable(${case} ${LA64_TX_TEST_DIR}/tx/regression/${case}.c)
    if(case STREQUAL "threadx_thread_wait_abort_and_isr_test")
        # The test's wall-clock budgets allow runs up to 300 s; keep the
        # generous limit so a slow environment cannot fail it spuriously.
        la64_register_regression_test(${case} 400)
    else()
        la64_register_regression_test(${case} 120)
    endif()
endforeach()

# Standalone kernel-setup test: its own main(), no testcontrol.
la64_add_regression_executable(threadx_initialize_kernel_setup_test
    ${LA64_TX_TEST_DIR}/tx/regression/threadx_initialize_kernel_setup_test.c)
la64_register_regression_test(threadx_initialize_kernel_setup_test 120)
