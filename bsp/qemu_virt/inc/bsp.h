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
uint32_t bsp_cpucfg_read(uint32_t word); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
void     bsp_timer_config_write(uint64_t tcfg); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
void     bsp_timer_ack(void); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
void     bsp_irq_line_enable(uint32_t line_mask); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
void     bsp_irq_line_disable(uint32_t line_mask); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
uint64_t bsp_crmd_read(void); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
uint64_t bsp_ecfg_read(void); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
uint64_t bsp_estat_read(void); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
uint64_t bsp_era_read(void); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
uint64_t bsp_badv_read(void); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
void     bsp_cpu_idle(void); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
void     bsp_cpu_halt(void); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
void     bsp_exception_entry_set(void (*entry)(void)); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
uint32_t bsp_iocsr_read32(uint32_t addr); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
void     bsp_iocsr_write32(uint32_t addr, uint32_t value); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */

/* UART console (LA-REQ-104).  */
void     bsp_uart_init(void);
void     bsp_uart_putc(char c);
void     bsp_uart_puts(const char *s);
void     bsp_uart_put_hex64(uint64_t value);
void     bsp_uart_put_dec(uint32_t value);

/* UART receive path (LA-REQ-104, LA-REQ-105).  */
void     bsp_uart_rx_irq_enable(void);
void     bsp_uart_rx_irq_disable(void);
uint32_t bsp_uart_rx_isr(void);
uint32_t bsp_uart_rx_available(void);
uint32_t bsp_uart_rx_read(uint8_t *buffer, uint32_t length);

typedef struct
{
    uint32_t isr_bytes;     /* bytes moved from RBR by the ISR */
    uint32_t isr_calls;     /* ISR invocations                */
    uint32_t max_drain;     /* largest single ISR drain       */
    uint32_t overflow;      /* bytes dropped: ring was full    */
    uint32_t line_errors;   /* LSR OE/PE/FE/BI observations    */
    uint64_t isr_crmd;      /* CRMD sampled inside the ISR     */
} bsp_uart_rx_stats_t;

void     bsp_uart_rx_stats_get(bsp_uart_rx_stats_t *stats);

/* External interrupt route: UART receive interrupt through the PCH-PIC
   and EXTIOI controllers to CPU interrupt line 2 (LA-REQ-105).  */
bool     bsp_external_irq_init(void);
void     bsp_external_irq_handler(void);
void     bsp_external_irq_notify_set(void (*notify)(void));

/* System tick (LA-REQ-102, LA-REQ-103).  */
bool     bsp_timer_init(uint32_t ticks_per_second);
uint64_t bsp_timer_frequency(void);

/* Power and test reporting (LA-REQ-107).  */
void     bsp_poweroff(void);
void     bsp_test_report(bool passed);
bool     bsp_stack_guard_check(void); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */

/* Fatal exception handling (LA-REQ-106).  */
void     bsp_fatal_exception(uint64_t estat, uint64_t era, uint64_t badv);

#endif /* BSP_H */
