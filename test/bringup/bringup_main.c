/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  M2 bring-up test: console, timer interrupt, interrupt posture control.

    Verifies: LA-REQ-002, LA-REQ-101, LA-REQ-102, LA-REQ-103, LA-REQ-104,
              LA-REQ-107  */

#include "bsp.h"
#include "tx_api.h"
#include "bringup.h"

#define TEST_TICKS          20U
#define ESTAT_IS_TIMER      (1ULL << BSP_IRQ_LINE_TIMER)
#define ESTAT_ECODE_MASK    (0x3FULL << 16)

static volatile uint32_t bringup_timer_ticks = 0U;

void bringup_trap_handler(uint64_t estat)
{
    if (((estat & ESTAT_ECODE_MASK) == 0U) && ((estat & ESTAT_IS_TIMER) != 0U))
    {
        bsp_timer_ack();
        bringup_timer_ticks = bringup_timer_ticks + 1U;
    }
    else
    {
        bsp_fatal_exception(estat, bsp_era_read(), bsp_badv_read());
    }
}

static bool check(bool condition, const char *name)
{
    bsp_uart_puts(condition ? "  PASS  " : "  FAIL  ");
    bsp_uart_puts(name);
    bsp_uart_puts("\n");
    return condition;
}

int main(void) /* parasoft-suppress MISRAC2025-RULE_8_6-a "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_5_8-b "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_8_3-c "separate freestanding images, one analysis project" */
{
    bool     ok = true;
    UINT     posture;
    uint32_t guard = 0U;
    uint32_t line_mask = 1U;

    bsp_uart_init();
    bsp_uart_puts("ThreadX LoongArch64 BSP bring-up (QEMU virt)\n");

    bsp_exception_entry_set(&bringup_trap_entry);

    ok = check(bsp_timer_init(BSP_TICKS_PER_SECOND_DEFAULT), "timer init") && ok;
    ok = check(bsp_timer_frequency() == 100000000ULL, "timer frequency 100 MHz") && ok;

    posture = _tx_thread_interrupt_control(TX_INT_DISABLE);
    ok = check((posture == TX_INT_DISABLE) &&
               ((bsp_crmd_read() & 0x4U) == 0U),
               "disable clears CRMD.IE and returns old posture") && ok;

    bsp_irq_line_enable(line_mask << BSP_IRQ_LINE_TIMER);
    posture = _tx_thread_interrupt_control(TX_INT_ENABLE);
    ok = check((posture == TX_INT_DISABLE) &&
               ((bsp_crmd_read() & 0x4U) != 0U),
               "enable sets CRMD.IE and returns old posture") && ok;

    while ((bringup_timer_ticks < TEST_TICKS) && (guard < 1000U))
    {
        bsp_cpu_idle();
        guard++;
    }
    ok = check(bringup_timer_ticks >= TEST_TICKS, "timer interrupts delivered") && ok;

    posture = _tx_thread_interrupt_control(TX_INT_DISABLE);
    ok = check((posture == TX_INT_ENABLE) &&
               ((bsp_crmd_read() & 0x4U) == 0U),
               "disable clears CRMD.IE and returns old posture") && ok;

    bsp_irq_line_disable(line_mask << BSP_IRQ_LINE_TIMER);
    ok = check((bsp_ecfg_read() & ((uint64_t) line_mask << BSP_IRQ_LINE_TIMER)) == 0U,
               "line disable clears ECFG.LIE") && ok;
    bsp_irq_line_enable(line_mask << BSP_IRQ_LINE_TIMER);
    ok = check((bsp_ecfg_read() & ((uint64_t) line_mask << BSP_IRQ_LINE_TIMER)) != 0U,
               "line enable sets ECFG.LIE") && ok;

    bsp_uart_puts("ticks=");
    bsp_uart_put_dec(bringup_timer_ticks);
    bsp_uart_puts("\n");

    bsp_test_report(ok);
    return 0;
}
