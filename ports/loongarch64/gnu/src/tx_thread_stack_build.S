/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

#include "tx_port.h"

/* LA-REQ-006, LA-REQ-008. */
    .section .text
    .globl  _tx_thread_stack_build
    .type   _tx_thread_stack_build, @function
_tx_thread_stack_build:
    /* The stack grows down.  Align the top before reserving the common
       interrupt frame used for a thread's first ERTN. */
    ld.d    $r12, $a0, 24
    li.d    $r13, -16
    and     $r12, $r12, $r13
    addi.d  $r12, $r12, -TX_LA64_CONTEXT_FRAME_SIZE

    st.d    $zero, $r12, 0
    st.d    $zero, $r12, 8
    st.d    $zero, $r12, 16
    st.d    $zero, $r12, 32
    st.d    $zero, $r12, 40
    st.d    $zero, $r12, 48
    st.d    $zero, $r12, 56
    st.d    $zero, $r12, 64
    st.d    $zero, $r12, 72
    st.d    $zero, $r12, 80
    st.d    $zero, $r12, 88
    st.d    $zero, $r12, 96
    st.d    $zero, $r12, 104
    st.d    $zero, $r12, 112
    st.d    $zero, $r12, 120
    st.d    $zero, $r12, 128
    st.d    $zero, $r12, 136
    st.d    $zero, $r12, 144
    st.d    $zero, $r12, 152
    st.d    $zero, $r12, 160
    st.d    $zero, $r12, 168
    st.d    $zero, $r12, 176
    st.d    $zero, $r12, 184
    st.d    $zero, $r12, 192
    st.d    $zero, $r12, 200
    st.d    $zero, $r12, 208
    st.d    $zero, $r12, 216
    st.d    $zero, $r12, 224
    st.d    $zero, $r12, 232
    st.d    $zero, $r12, 240
    st.d    $zero, $r12, 248
    addi.d  $r13, $r12, TX_LA64_CONTEXT_FRAME_SIZE
    st.d    $r13, $r12, 24
    st.d    $a1, $r12, TX_LA64_CONTEXT_ERA_OFFSET
    li.d    $r13, TX_LA64_CRMD_IE
    st.d    $r13, $r12, TX_LA64_CONTEXT_PRMD_OFFSET

    st.d    $zero, $r12, 272
    st.d    $zero, $r12, 280
    st.d    $zero, $r12, 288
    st.d    $zero, $r12, 296
    st.d    $zero, $r12, 304
    st.d    $zero, $r12, 312
    st.d    $zero, $r12, 320
    st.d    $zero, $r12, 328
    st.d    $zero, $r12, 336
    st.d    $zero, $r12, 344
    st.d    $zero, $r12, 352
    st.d    $zero, $r12, 360
    st.d    $zero, $r12, 368
    st.d    $zero, $r12, 376
    st.d    $zero, $r12, 384
    st.d    $zero, $r12, 392
    st.d    $zero, $r12, 400
    st.d    $zero, $r12, 408
    st.d    $zero, $r12, 416
    st.d    $zero, $r12, 424
    st.d    $zero, $r12, 432
    st.d    $zero, $r12, 440
    st.d    $zero, $r12, 448
    st.d    $zero, $r12, 456
    st.d    $zero, $r12, 464
    st.d    $zero, $r12, 472
    st.d    $zero, $r12, 480
    st.d    $zero, $r12, 488
    st.d    $zero, $r12, 496
    st.d    $zero, $r12, 504
    st.d    $zero, $r12, 512
    st.d    $zero, $r12, 520
    st.d    $zero, $r12, TX_LA64_CONTEXT_FCSR_OFFSET
    st.d    $zero, $r12, 536
    st.d    $zero, $r12, 544
    st.d    $zero, $r12, 552
    st.d    $zero, $r12, 560
    st.d    $zero, $r12, 568
    st.d    $zero, $r12, 576
    st.d    $zero, $r12, 584
    st.d    $zero, $r12, 592
    st.d    $zero, $r12, 600

    li.d    $r13, TX_LA64_CONTEXT_INTERRUPT_TYPE
    st.d    $r13, $r12, 0
    st.d    $r12, $a0, 8
    jr      $ra
    .size   _tx_thread_stack_build, . - _tx_thread_stack_build
