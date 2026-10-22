/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Reset entry for the QEMU loongarch64 "virt" machine.

    QEMU loads the ELF image directly into RAM and starts at _start in
    direct address mode (CRMD.DA=1, PG=0) at PLV0 with interrupts disabled.
    .data is therefore already at its run address and only .bss is cleared.

    Requirements: LA-REQ-101, LA-REQ-106, LA-REQ-012  */

#define CSR_CRMD    0x0
#define CSR_EUEN    0x2
#define CSR_EENTRY  0xc
#define CSR_ESTAT   0x5
#define CSR_ERA     0x6
#define CSR_BADV    0x7

    .section .text.start, "ax"
    .globl  _start
    .type   _start, @function
_start:
    /* Interrupts off (CRMD.IE = 0), independent of the loader state.  */
    li.w    $t0, 0x4
    csrxchg $zero, $t0, CSR_CRMD

    la.abs  $sp, __system_stack_top

    /* Clear .bss (8-byte aligned by the linker script).  */
    la.abs  $t0, __bss_start
    la.abs  $t1, __bss_end
1:  bgeu    $t0, $t1, 2f
    st.d    $zero, $t0, 0
    addi.d  $t0, $t0, 8
    b       1b
2:
    /* Fill the guard below the system stack after .bss is cleared.  */
    la.abs  $t0, __system_stack_guard_start
    la.abs  $t1, __system_stack_guard_end
    li.d    $t2, 0x5AA55AA55AA55AA5
3:  bgeu    $t0, $t1, 4f
    st.d    $t2, $t0, 0
    addi.d  $t0, $t0, 8
    b       3b
4:
    /* Enable the FPU (EUEN.FPE) and clear FCSR0.  */
    li.w    $t0, 0x1
    csrxchg $t0, $t0, CSR_EUEN
    movgr2fcsr $fcsr0, $zero

    /* Until the port installs its own entry, any exception is fatal.  */
    la.abs  $t0, bsp_default_trap
    csrwr   $t0, CSR_EENTRY

#ifdef CPPTEST_COVERAGE
    /* Initialise Parasoft's freestanding coverage runtime before main.  */
    bl      CppTest_InitializeRuntime
#endif

    bl      main

    /* main must not return.  */
    bl      bsp_poweroff
5:  b       5b
    .size   _start, . - _start

/* Default exception entry.  EENTRY requires 4 KiB alignment.  */
    .section .text.trap, "ax"
    .balign 4096
    .globl  bsp_default_trap
    .type   bsp_default_trap, @function
bsp_default_trap:
    csrrd   $a0, CSR_ESTAT
    csrrd   $a1, CSR_ERA
    csrrd   $a2, CSR_BADV
    la.abs  $sp, __system_stack_top
    bl      bsp_fatal_exception
6:  b       6b
    .size   bsp_default_trap, . - bsp_default_trap
