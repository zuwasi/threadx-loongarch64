/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

#include "tx_port.h"

    .section .text
    .globl  _tx_thread_schedule
    .type   _tx_thread_schedule, @function
_tx_thread_schedule:
_tx_thread_schedule_loop:
    /* Enable interrupts while waiting for a ready thread. */
    li.w    $r12, TX_LA64_CRMD_IE
    move    $r13, $r12
    csrxchg $r12, $r13, TX_LA64_CSR_CRMD

    la.abs  $r12, _tx_thread_execute_ptr
    ld.d    $r13, $r12, 0
    beqz    $r13, _tx_thread_schedule_idle

    /* Lock interrupts before committing the selected thread. */
    move    $r12, $zero
    li.w    $r14, TX_LA64_CRMD_IE
    csrxchg $r12, $r14, TX_LA64_CSR_CRMD

    la.abs  $r12, _tx_thread_current_ptr
    st.d    $r13, $r12, 0

    ld.w    $r14, $r13, 4
    addi.w  $r14, $r14, 1
    st.w    $r14, $r13, 4

    ld.w    $r14, $r13, 36
    la.abs  $r12, _tx_timer_time_slice
    st.w    $r14, $r12, 0

    ld.d    $sp, $r13, 8
#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    bl      _tx_execution_thread_enter
#endif
    ld.d    $r12, $sp, 0
    beqz    $r12, _tx_thread_solicited_return
    b       _tx_la64_context_restore

_tx_thread_schedule_idle:
    idle    0
    b       _tx_thread_schedule_loop

/* LA-REQ-005: restore only ABI callee-saved state from a voluntary return. */
_tx_thread_solicited_return:
    move    $r21, $sp
    fld.d   $f24, $r21, 104
    fld.d   $f25, $r21, 112
    fld.d   $f26, $r21, 120
    fld.d   $f27, $r21, 128
    fld.d   $f28, $r21, 136
    fld.d   $f29, $r21, 144
    fld.d   $f30, $r21, 152
    fld.d   $f31, $r21, 160
    ld.d    $r12, $r21, TX_LA64_SOLICITED_FCSR_OFFSET
    movgr2fcsr $fcsr0, $r12
    ld.d    $r12, $r21, TX_LA64_SOLICITED_PRMD_OFFSET
    ld.d    $ra,  $r21, 8
    ld.d    $r22, $r21, 24
    ld.d    $r23, $r21, 32
    ld.d    $r24, $r21, 40
    ld.d    $r25, $r21, 48
    ld.d    $r26, $r21, 56
    ld.d    $r27, $r21, 64
    ld.d    $r28, $r21, 72
    ld.d    $r29, $r21, 80
    ld.d    $r30, $r21, 88
    ld.d    $r31, $r21, 96
    ld.d    $sp,  $r21, 16
    move    $r13, $r12
    li.w    $r12, TX_LA64_CRMD_IE
    csrxchg $r13, $r12, TX_LA64_CSR_CRMD
    jr      $ra
    .size   _tx_thread_schedule, . - _tx_thread_schedule
