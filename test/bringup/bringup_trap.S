/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Bring-up test exception entry.

    Saves the caller-saved integer registers, calls bringup_trap_handler
    with ESTAT, restores and returns with ertn.  Used only by the M2
    bring-up test, before the port's context save is in place.  The C
    handler does not use floating point.  */

#define FRAME_SIZE  160

    .section .text.trap.bringup, "ax"
    .balign 4096
    .globl  bringup_trap_entry
    .type   bringup_trap_entry, @function
bringup_trap_entry:
    addi.d  $sp, $sp, -FRAME_SIZE
    st.d    $ra,  $sp, 0
    st.d    $a0,  $sp, 8
    st.d    $a1,  $sp, 16
    st.d    $a2,  $sp, 24
    st.d    $a3,  $sp, 32
    st.d    $a4,  $sp, 40
    st.d    $a5,  $sp, 48
    st.d    $a6,  $sp, 56
    st.d    $a7,  $sp, 64
    st.d    $t0,  $sp, 72
    st.d    $t1,  $sp, 80
    st.d    $t2,  $sp, 88
    st.d    $t3,  $sp, 96
    st.d    $t4,  $sp, 104
    st.d    $t5,  $sp, 112
    st.d    $t6,  $sp, 120
    st.d    $t7,  $sp, 128
    st.d    $t8,  $sp, 136
    st.d    $r21, $sp, 144

    csrrd   $a0, 0x5
    bl      bringup_trap_handler

    ld.d    $ra,  $sp, 0
    ld.d    $a0,  $sp, 8
    ld.d    $a1,  $sp, 16
    ld.d    $a2,  $sp, 24
    ld.d    $a3,  $sp, 32
    ld.d    $a4,  $sp, 40
    ld.d    $a5,  $sp, 48
    ld.d    $a6,  $sp, 56
    ld.d    $a7,  $sp, 64
    ld.d    $t0,  $sp, 72
    ld.d    $t1,  $sp, 80
    ld.d    $t2,  $sp, 88
    ld.d    $t3,  $sp, 96
    ld.d    $t4,  $sp, 104
    ld.d    $t5,  $sp, 112
    ld.d    $t6,  $sp, 120
    ld.d    $t7,  $sp, 128
    ld.d    $t8,  $sp, 136
    ld.d    $r21, $sp, 144
    addi.d  $sp, $sp, FRAME_SIZE
    ertn
    .size   bringup_trap_entry, . - bringup_trap_entry

