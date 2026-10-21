/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

#include "tx_port.h"
#include "tx_la64_context.inc"

/* LA-REQ-004, LA-REQ-007, LA-REQ-009. */
    .section .text
    .globl  _tx_thread_context_restore
    .type   _tx_thread_context_restore, @function
_tx_thread_context_restore:
#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    addi.d  $sp, $sp, -16
    st.d    $ra, $sp, 8
    bl      _tx_execution_isr_exit
    ld.d    $ra, $sp, 8
    addi.d  $sp, $sp, 16
#endif
    la.abs  $r12, _tx_thread_system_state
    ld.w    $r13, $r12, 0
    addi.w  $r13, $r13, -1
    st.w    $r13, $r12, 0
    bnez    $r13, _tx_la64_context_restore

    la.abs  $r12, _tx_thread_current_ptr
    ld.d    $r13, $r12, 0
    beqz    $r13, _tx_thread_schedule

    la.abs  $r12, _tx_thread_preempt_disable
    ld.w    $r14, $r12, 0
    bnez    $r14, _tx_thread_context_restore_current

    la.abs  $r12, _tx_thread_execute_ptr
    ld.d    $r14, $r12, 0
    beq     $r13, $r14, _tx_thread_context_restore_current

    /* The interrupt frame is already complete, so it can be retained as
       the thread's resumable context without copying register state. */
    la.abs  $r12, _tx_timer_time_slice
    ld.w    $r14, $r12, 0
    beqz    $r14, 1f
    st.w    $r14, $r13, 36
    st.w    $zero, $r12, 0
1:  la.abs  $r12, _tx_thread_current_ptr
    st.d    $zero, $r12, 0
    b       _tx_thread_schedule

_tx_thread_context_restore_current:
    ld.d    $sp, $r13, 8
    b       _tx_la64_context_restore

    .globl  _tx_la64_context_restore
    .type   _tx_la64_context_restore, @function
_tx_la64_context_restore:
    move    $r21, $sp
    ld.d    $r12, $r21, TX_LA64_CONTEXT_ERA_OFFSET
    csrwr   $r12, TX_LA64_CSR_ERA
    ld.d    $r12, $r21, TX_LA64_CONTEXT_PRMD_OFFSET
    csrwr   $r12, TX_LA64_CSR_PRMD
    tx_la64_restore_fprs
    tx_la64_restore_gprs
    ertn
    .size   _tx_la64_context_restore, . - _tx_la64_context_restore
    .size   _tx_thread_context_restore, . - _tx_thread_context_restore
