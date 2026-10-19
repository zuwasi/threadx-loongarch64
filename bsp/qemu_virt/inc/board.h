/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  QEMU loongarch64 "virt" machine: memory map and constants.

    Values verified against QEMU 8.2.2 (-M virt -cpu la464).  */

#ifndef BSP_BOARD_H
#define BSP_BOARD_H

#include <stdint.h>

/* NS16550-compatible UART 0, byte registers (reg-shift 0).  */
#define BSP_UART0_BASE                  0x1FE001E0UL

/* ACPI GED sleep control register.  Writing SLP_TYP=S5 | SLP_EN powers the
   machine off.  */
#define BSP_GED_SLEEP_CTL_ADDR          0x100E001CUL
#define BSP_GED_SLEEP_CTL_POWEROFF      0x34U

/* Default ThreadX tick rate.  */
#define BSP_TICKS_PER_SECOND_DEFAULT    100U

/* CPU interrupt lines (ECFG.LIE / ESTAT.IS bit numbers).  */
#define BSP_IRQ_LINE_TIMER              11U

#endif /* BSP_BOARD_H */
