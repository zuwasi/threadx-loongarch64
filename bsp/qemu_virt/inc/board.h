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
#define BSP_IRQ_LINE_EXTERNAL           2U

/* PCH-PIC (LS7A1000-compatible I/O interrupt controller), normal MMIO.
   Offsets verified against QEMU 8.2.2 hw/intc/loongarch_pch_pic.c.  A set
   INT_MASK bit masks the line; the reset value masks every line.  */
#define BSP_PCH_PIC_BASE                0x10000000UL
#define BSP_PCH_PIC_REG_INT_MASK        0x20UL
#define BSP_PCH_PIC_REG_HTMSI_VEC       0x200UL

/* UART interrupt line at the PCH-PIC.  QEMU 8.2.2 hw/loongarch/virt.c
   wires the ns16550 to pch_pic input VIRT_UART_IRQ - VIRT_GSI_BASE = 2.  */
#define BSP_UART_PCH_IRQ                2U

/* EXTIOI (3A5000 external interrupt controller), in IOCSR space.  Offsets
   are relative to BSP_EXTIOI_IOCSR_BASE and match QEMU 8.2.2
   hw/intc/loongarch_extioi.c.  Input 2 reaches CPU line
   BSP_IRQ_LINE_EXTERNAL through IP0.  */
#define BSP_EXTIOI_IOCSR_BASE           0x1400UL
#define BSP_EXTIOI_REG_IPMAP            0x0C0UL
#define BSP_EXTIOI_REG_ENABLE           0x200UL
#define BSP_EXTIOI_REG_COREISR          0x400UL
#define BSP_EXTIOI_REG_COREMAP          0x800UL

#endif /* BSP_BOARD_H */
