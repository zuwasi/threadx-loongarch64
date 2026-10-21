/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

#include "tx_port.h"

/* LA-REQ-011, LA-REQ-012, LA-REQ-008. */
    .section .text
    .globl  _tx_initialize_low_level
    .weak   _tx_initialize_low_level
    .type   _tx_initialize_low_level, @function
_tx_initialize_low_level:
    la.abs  $r12, _tx_thread_system_stack_ptr
    st.d    $sp, $r12, 0

    addi.d  $sp, $sp, -16
    st.d    $ra, $sp, 8

    /* Enable the LP64D FPU before any context frame is restored. */
    li.w    $r12, 1
    csrxchg $r12, $r12, TX_LA64_CSR_EUEN
    movgr2fcsr $fcsr0, $zero

    la.abs  $r12, __free_memory_start
    la.abs  $r13, _tx_initialize_unused_memory
    st.d    $r12, $r13, 0

    la.abs  $r12, _tx_thread_vector_entry
    csrwr   $r12, TX_LA64_CSR_EENTRY

    /* Only the periodic timer source is supported by this single-core BSP. */
    li.w    $a0, 0x1FFF
    bl      bsp_irq_line_disable
    li.w    $a0, TX_TIMER_TICKS_PER_SECOND
    bl      bsp_timer_init
    beqz    $a0, _tx_initialize_low_level_error

    li.w    $a0, 0x800
    bl      bsp_irq_line_enable
    ld.d    $ra, $sp, 8
    addi.d  $sp, $sp, 16
    jr      $ra

_tx_initialize_low_level_error:
    move    $a0, $zero
    move    $a1, $zero
    move    $a2, $zero
    bl      bsp_fatal_exception
1:  b       1b
    .size   _tx_initialize_low_level, . - _tx_initialize_low_level
