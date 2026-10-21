/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

#include "tx_port.h"

/* LA-REQ-005, LA-REQ-008. */
    .section .text
    .globl  _tx_thread_system_return
    .type   _tx_thread_system_return, @function
_tx_thread_system_return:
    /* Some kernel paths (for example _tx_thread_relinquish) call this with
       interrupts enabled.  Record the caller's posture for the solicited
       return, then lock out interrupts before any bookkeeping: an interrupt
       taken after the switch to the system stack below, while
       _tx_thread_current_ptr still names this thread, would save a
       system-stack frame as the thread's context (PR-M5-001).  */
    csrrd   $r15, TX_LA64_CSR_CRMD
    andi    $r15, $r15, 0x7
    move    $r12, $zero
    li.w    $r13, TX_LA64_CRMD_IE
    csrxchg $r12, $r13, TX_LA64_CSR_CRMD

    /* The solicited frame contains only the ABI callee-saved registers
       plus frame metadata. */
    addi.d  $sp, $sp, -TX_LA64_SOLICITED_FRAME_SIZE
    st.d    $zero, $sp, 0
    st.d    $ra, $sp, 8
    addi.d  $r12, $sp, TX_LA64_SOLICITED_FRAME_SIZE
    st.d    $r12, $sp, 16
    st.d    $r22, $sp, 24
    st.d    $r23, $sp, 32
    st.d    $r24, $sp, 40
    st.d    $r25, $sp, 48
    st.d    $r26, $sp, 56
    st.d    $r27, $sp, 64
    st.d    $r28, $sp, 72
    st.d    $r29, $sp, 80
    st.d    $r30, $sp, 88
    st.d    $r31, $sp, 96
    fst.d   $f24, $sp, 104
    fst.d   $f25, $sp, 112
    fst.d   $f26, $sp, 120
    fst.d   $f27, $sp, 128
    fst.d   $f28, $sp, 136
    fst.d   $f29, $sp, 144
    fst.d   $f30, $sp, 152
    fst.d   $f31, $sp, 160
    movfcsr2gr $r12, $fcsr0
    st.d    $r12, $sp, TX_LA64_SOLICITED_FCSR_OFFSET
    st.d    $r15, $sp, TX_LA64_SOLICITED_PRMD_OFFSET

    la.abs  $r12, _tx_thread_current_ptr
    ld.d    $r13, $r12, 0
    st.d    $sp, $r13, 8
#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    addi.d  $sp, $sp, -16
    st.d    $ra, $sp, 8
    bl      _tx_execution_thread_exit
    ld.d    $ra, $sp, 8
    addi.d  $sp, $sp, 16
    la.abs  $r12, _tx_thread_current_ptr
    ld.d    $r13, $r12, 0
#endif
    la.abs  $r12, _tx_thread_system_stack_ptr
    ld.d    $sp, $r12, 0

    la.abs  $r12, _tx_timer_time_slice
    ld.w    $r14, $r12, 0
    beqz    $r14, 1f
    st.w    $r14, $r13, 36
    st.w    $zero, $r12, 0
1:  la.abs  $r12, _tx_thread_current_ptr
    st.d    $zero, $r12, 0
    b       _tx_thread_schedule
    .size   _tx_thread_system_return, . - _tx_thread_system_return
