/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  CPU control and status register access for the BSP.

    Each function wraps one operation so that C code contains no assembly
    (MISRA C:2025 Dir 4.3).  LoongArch CSR numbers are immediates, so each
    register needs its own function.

    Requirements: LA-REQ-102, LA-REQ-103, LA-REQ-106  */

#define CSR_EUEN    0x2
#define CSR_CRMD    0x0
#define CSR_ECFG    0x4
#define CSR_ESTAT   0x5
#define CSR_ERA     0x6
#define CSR_BADV    0x7
#define CSR_TCFG    0x41
#define CSR_TICLR   0x44

    .text

/* uint32_t bsp_cpucfg_read(uint32_t word)  */
    .globl  bsp_cpucfg_read
    .type   bsp_cpucfg_read, @function
bsp_cpucfg_read:
    cpucfg  $a0, $a0
    jr      $ra
    .size   bsp_cpucfg_read, . - bsp_cpucfg_read

/* void bsp_timer_config_write(uint64_t tcfg)  */
    .globl  bsp_timer_config_write
    .type   bsp_timer_config_write, @function
bsp_timer_config_write:
    csrwr   $a0, CSR_TCFG
    jr      $ra
    .size   bsp_timer_config_write, . - bsp_timer_config_write

/* void bsp_timer_ack(void): TICLR.CLR = 1  */
    .globl  bsp_timer_ack
    .type   bsp_timer_ack, @function
bsp_timer_ack:
    li.w    $t0, 1
    csrwr   $t0, CSR_TICLR
    jr      $ra
    .size   bsp_timer_ack, . - bsp_timer_ack

/* void bsp_irq_line_enable(uint32_t line_mask): set ECFG.LIE bits  */
    .globl  bsp_irq_line_enable
    .type   bsp_irq_line_enable, @function
bsp_irq_line_enable:
    csrxchg $a0, $a0, CSR_ECFG
    jr      $ra
    .size   bsp_irq_line_enable, . - bsp_irq_line_enable

/* void bsp_irq_line_disable(uint32_t line_mask): clear ECFG.LIE bits  */
    .globl  bsp_irq_line_disable
    .type   bsp_irq_line_disable, @function
bsp_irq_line_disable:
    move    $t0, $zero
    csrxchg $t0, $a0, CSR_ECFG
    jr      $ra
    .size   bsp_irq_line_disable, . - bsp_irq_line_disable

/* uint64_t bsp_crmd_read(void)  */
    .globl  bsp_crmd_read
    .type   bsp_crmd_read, @function
bsp_crmd_read:
    csrrd   $a0, CSR_CRMD
    jr      $ra
    .size   bsp_crmd_read, . - bsp_crmd_read

/* uint64_t bsp_ecfg_read(void)  */
    .globl  bsp_ecfg_read
    .type   bsp_ecfg_read, @function
bsp_ecfg_read:
    csrrd   $a0, CSR_ECFG
    jr      $ra
    .size   bsp_ecfg_read, . - bsp_ecfg_read

/* uint64_t bsp_estat_read(void)  */
    .globl  bsp_estat_read
    .type   bsp_estat_read, @function
bsp_estat_read:
    csrrd   $a0, CSR_ESTAT
    jr      $ra
    .size   bsp_estat_read, . - bsp_estat_read

/* uint64_t bsp_era_read(void)  */
    .globl  bsp_era_read
    .type   bsp_era_read, @function
bsp_era_read:
    csrrd   $a0, CSR_ERA
    jr      $ra
    .size   bsp_era_read, . - bsp_era_read

/* uint64_t bsp_badv_read(void)  */
    .globl  bsp_badv_read
    .type   bsp_badv_read, @function
bsp_badv_read:
    csrrd   $a0, CSR_BADV
    jr      $ra
    .size   bsp_badv_read, . - bsp_badv_read

/* void bsp_cpu_idle(void): wait for an interrupt  */
    .globl  bsp_cpu_idle
    .type   bsp_cpu_idle, @function
bsp_cpu_idle:
    idle    0
    jr      $ra
    .size   bsp_cpu_idle, . - bsp_cpu_idle

/* void bsp_cpu_halt(void): disable interrupts and stop  */
    .globl  bsp_cpu_halt
    .type   bsp_cpu_halt, @function
bsp_cpu_halt:
    li.w    $t0, 0x4
    csrxchg $zero, $t0, CSR_CRMD
1:  idle    0
    b       1b
    .size   bsp_cpu_halt, . - bsp_cpu_halt

/* void bsp_exception_entry_set(void (*entry)(void)): EENTRY, 4 KiB aligned  */
    .globl  bsp_exception_entry_set
    .type   bsp_exception_entry_set, @function
bsp_exception_entry_set:
    csrwr   $a0, 0xc
    jr      $ra
    .size   bsp_exception_entry_set, . - bsp_exception_entry_set
