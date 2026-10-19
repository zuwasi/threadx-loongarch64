/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  BSP interface for the QEMU loongarch64 "virt" machine.

    Requirements: LA-REQ-101 to LA-REQ-107  */

#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include <stdbool.h>
#include "board.h"

/* CPU access, implemented in assembly (bsp_cpu.S).  All control and status
   register access is confined there (MISRA C:2025 Dir 4.3).  */
uint32_t bsp_cpucfg_read(uint32_t word);
void     bsp_timer_config_write(uint64_t tcfg);
void     bsp_timer_ack(void);
void     bsp_irq_line_enable(uint32_t line_mask);
void     bsp_irq_line_disable(uint32_t line_mask);
uint64_t bsp_estat_read(void);
uint64_t bsp_era_read(void);
uint64_t bsp_badv_read(void);
void     bsp_cpu_idle(void);
void     bsp_cpu_halt(void);
void     bsp_exception_entry_set(uintptr_t entry);

/* UART console (LA-REQ-104).  */
void     bsp_uart_init(void);
void     bsp_uart_putc(char c);
void     bsp_uart_puts(const char *s);
void     bsp_uart_put_hex64(uint64_t value);
void     bsp_uart_put_dec(uint32_t value);

/* System tick (LA-REQ-102, LA-REQ-103).  */
bool     bsp_timer_init(uint32_t ticks_per_second);
uint64_t bsp_timer_frequency(void);

/* Power and test reporting (LA-REQ-107).  */
void     bsp_poweroff(void);
void     bsp_test_report(bool passed);

/* Fatal exception handling (LA-REQ-106).  */
void     bsp_fatal_exception(uint64_t estat, uint64_t era, uint64_t badv);

#endif /* BSP_H */
