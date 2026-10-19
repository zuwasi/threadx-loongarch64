/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  M2 bring-up test: console, timer interrupt, interrupt posture control.

    Verifies: LA-REQ-002, LA-REQ-101, LA-REQ-102, LA-REQ-103, LA-REQ-104,
              LA-REQ-107  */

#include "bsp.h"
#include "tx_api.h"

#define TEST_TICKS          20U
#define ESTAT_IS_TIMER      (1ULL << BSP_IRQ_LINE_TIMER)
#define ESTAT_ECODE_MASK    (0x3FULL << 16)

extern void bringup_trap_entry(void);
extern void bsp_exception_entry_set(uintptr_t entry);
void        bringup_trap_handler(uint64_t estat);

static volatile uint32_t timer_ticks = 0U;

void bringup_trap_handler(uint64_t estat)
{
    if (((estat & ESTAT_ECODE_MASK) == 0U) && ((estat & ESTAT_IS_TIMER) != 0U))
    {
        bsp_timer_ack();
        timer_ticks++;
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

int main(void)
{
    bool     ok = true;
    UINT     posture;
    uint32_t guard = 0U;

    bsp_uart_init();
    bsp_uart_puts("ThreadX LoongArch64 BSP bring-up (QEMU virt)\n");

    bsp_exception_entry_set((uintptr_t) &bringup_trap_entry);

    ok = check(bsp_timer_init(BSP_TICKS_PER_SECOND_DEFAULT), "timer init") && ok;
    ok = check(bsp_timer_frequency() == 100000000ULL, "timer frequency 100 MHz") && ok;

    posture = _tx_thread_interrupt_control(TX_INT_DISABLE);
    ok = check(posture == TX_INT_DISABLE, "posture after reset is disabled") && ok;

    bsp_irq_line_enable(1U << BSP_IRQ_LINE_TIMER);
    posture = _tx_thread_interrupt_control(TX_INT_ENABLE);
    ok = check(posture == TX_INT_DISABLE, "enable returns previous posture") && ok;

    while ((timer_ticks < TEST_TICKS) && (guard < 1000U))
    {
        bsp_cpu_idle();
        guard++;
    }
    ok = check(timer_ticks >= TEST_TICKS, "timer interrupts delivered") && ok;

    posture = _tx_thread_interrupt_control(TX_INT_DISABLE);
    ok = check(posture == TX_INT_ENABLE, "disable returns previous posture") && ok;

    bsp_uart_puts("ticks=");
    bsp_uart_put_dec(timer_ticks);
    bsp_uart_puts("\n");

    bsp_test_report(ok);
    return 0;
}
