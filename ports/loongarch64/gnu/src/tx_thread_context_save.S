/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

#include "tx_port.h"
#include "tx_la64_context.inc"

/* LA-REQ-003, LA-REQ-009, LA-REQ-012, LA-REQ-103, LA-REQ-106. */
    .section .text.trap.port, "ax"
    .balign 4096
    .globl  _tx_thread_vector_entry
    .type   _tx_thread_vector_entry, @function
_tx_thread_vector_entry:
    /* SAVE0 preserves the interrupted stack pointer without clobbering a
       general register.  Hardware has already cleared CRMD.IE. */
    csrwr   $sp, TX_LA64_CSR_SAVE0
    csrrd   $sp, TX_LA64_CSR_SAVE0
    addi.d  $sp, $sp, -TX_LA64_CONTEXT_FRAME_SIZE

    st.d    $zero, $sp, 0
    tx_la64_save_gprs
    csrrd   $r12, TX_LA64_CSR_ERA
    st.d    $r12, $sp, TX_LA64_CONTEXT_ERA_OFFSET
    csrrd   $r12, TX_LA64_CSR_PRMD
    st.d    $r12, $sp, TX_LA64_CONTEXT_PRMD_OFFSET
    tx_la64_save_fprs
    li.w    $r12, TX_LA64_CONTEXT_INTERRUPT_TYPE
    st.d    $r12, $sp, 0

    /* Only the enabled timer interrupt is handled by this port.  Other
       exceptions, including unexpected interrupt sources, are fatal. */
    csrrd   $r12, TX_LA64_CSR_ESTAT
    srli.d  $r13, $r12, 16
    andi    $r13, $r13, 0x3F
    bnez    $r13, _tx_thread_vector_fatal
    andi    $r13, $r12, 0x800
    beqz    $r13, _tx_thread_vector_fatal

    bl      bsp_timer_ack
    bl      _tx_thread_context_save
    bl      _tx_timer_interrupt
    b       _tx_thread_context_restore

_tx_thread_vector_fatal:
    csrrd   $a0, TX_LA64_CSR_ESTAT
    csrrd   $a1, TX_LA64_CSR_ERA
    csrrd   $a2, 0x7
    la.abs  $r12, _tx_thread_system_stack_ptr
    ld.d    $sp, $r12, 0
    bl      bsp_fatal_exception
1:  b       1b
    .size   _tx_thread_vector_entry, . - _tx_thread_vector_entry

    .section .text, "ax"
    .globl  _tx_thread_context_save
    .type   _tx_thread_context_save, @function
_tx_thread_context_save:
    la.abs  $r12, _tx_thread_system_state
    ld.w    $r13, $r12, 0
    addi.w  $r14, $r13, 1
    st.w    $r14, $r12, 0

    /* A non-zero prior state means this frame is nested on the active
       system/handler stack.  Do not replace the outer frame's stack. */
    bnez    $r13, 2f

    la.abs  $r12, _tx_thread_current_ptr
    ld.d    $r13, $r12, 0
    beqz    $r13, 1f

    st.d    $sp, $r13, 8
    la.abs  $r12, _tx_thread_system_stack_ptr
    ld.d    $sp, $r12, 0
    b       2f

1:
    /* Interrupted the scheduler idle loop.  _tx_thread_context_restore
       re-enters the scheduler instead of restoring this frame, so drop it
       by restarting on the system stack.  Keeping it would leak one frame
       per idle tick and overflow the system stack.  */
    la.abs  $r12, _tx_thread_system_stack_ptr
    ld.d    $sp, $r12, 0
2:
#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    addi.d  $sp, $sp, -16
    st.d    $ra, $sp, 8
    bl      _tx_execution_isr_enter
    ld.d    $ra, $sp, 8
    addi.d  $sp, $sp, 16
#endif
    jr      $ra
    .size   _tx_thread_context_save, . - _tx_thread_context_save
