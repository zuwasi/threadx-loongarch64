/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  M7 bring-up test: route the UART receive interrupt through the PCH-PIC
    and EXTIOI controllers to CPU interrupt line 2, then receive a payload
    the host sends through QEMU's serial input.

    Verifies: LA-REQ-002, LA-REQ-009, LA-REQ-104, LA-REQ-105, LA-REQ-107  */

#include "tx_api.h"
#include "bsp.h"
#include <string.h>

#define RX_PAYLOAD_LEN   ((uint32_t) (sizeof(rx_expected) - 1U))
#define TEST_STACK_SIZE  2048U
#define TEST_PRIORITY    5U
#define RX_WAIT_TICKS    300U
#define CRMD_IE_MASK     0x4ULL

static TX_SEMAPHORE rx_semaphore;
static volatile ULONG rx_isr_notifications;
static volatile ULONG rx_put_failures;

static bool rx_check(bool condition, const char *name)
{
    bsp_uart_puts(condition ? "  PASS  " : "  FAIL  ");
    bsp_uart_puts(name);
    bsp_uart_puts("\n");
    return condition;
}

static VOID rx_isr_notify(void)
{
    UINT status;

    rx_isr_notifications = rx_isr_notifications + 1U;
    status = tx_semaphore_put(&rx_semaphore);
    if (status != TX_SUCCESS)
    {
        rx_put_failures = rx_put_failures + 1U;
    }
}

static VOID rx_test_entry(ULONG thread_input)
{
    /* Keep byte-identical to test/bringup/uart_rx_payload.txt.  */
    static const uint8_t rx_expected[] =
        "ThreadX-LoongArch64 UART RX interrupt route payload 0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
    bsp_uart_rx_stats_t stats;
    bool     ok = true;
    UINT     status;
    ULONG    start_tick;
    ULONG    end_tick;
    ULONG    rx_isr_wakeups = 0U;
    uint32_t elapsed;
    uint32_t received;
    uint8_t  received_data[sizeof(rx_expected)];

    (void) thread_input;

    ok = rx_check(bsp_external_irq_init(), "external interrupt route init") && ok;

    bsp_external_irq_notify_set(&rx_isr_notify);
    bsp_irq_line_enable(1U << BSP_IRQ_LINE_EXTERNAL);
    ok = rx_check((bsp_ecfg_read() & (1ULL << BSP_IRQ_LINE_EXTERNAL)) != 0ULL,
                  "CPU line 2 enabled in ECFG.LIE") && ok;

    bsp_uart_rx_irq_enable();
    bsp_uart_puts("RX READY\n");

    start_tick = tx_time_get();
    while ((bsp_uart_rx_available() < RX_PAYLOAD_LEN) &&
           ((tx_time_get() - start_tick) < RX_WAIT_TICKS))
    {
        status = tx_semaphore_get(&rx_semaphore, 2U);
        if (status == TX_SUCCESS)
        {
            rx_isr_wakeups = rx_isr_wakeups + 1U;
        }
    }
    end_tick = tx_time_get(); /* parasoft-suppress MISRAC2025-DIR_4_7-a "DEV-015" */
    elapsed = (uint32_t) (end_tick - start_tick);

    received = bsp_uart_rx_read(received_data, RX_PAYLOAD_LEN);
    ok = rx_check(received == RX_PAYLOAD_LEN, "payload length received") && ok;
    ok = rx_check(memcmp(received_data, rx_expected, (size_t) received) == 0,
                  "payload content") && ok;

    bsp_uart_rx_stats_get(&stats);
    ok = rx_check(stats.isr_calls >= 1U, "UART RX ISR invoked") && ok;
    ok = rx_check(stats.isr_bytes == received, "ISR moved every byte") && ok;
    ok = rx_check(stats.overflow == 0U, "no ring overflow") && ok;
    ok = rx_check(stats.line_errors == 0U, "no UART line errors") && ok;
    ok = rx_check((stats.isr_calls != 0U) && ((stats.isr_crmd & CRMD_IE_MASK) == 0ULL),
                  "interrupts disabled inside the ISR") && ok;
    ok = rx_check(rx_isr_notifications >= 1U, "ISR notified the test thread") && ok;
    ok = rx_check(rx_put_failures == 0U, "semaphore put from ISR succeeded") && ok;

    bsp_uart_puts("rx isr_calls=");
    bsp_uart_put_dec(stats.isr_calls);
    bsp_uart_puts(" isr_bytes=");
    bsp_uart_put_dec(stats.isr_bytes);
    bsp_uart_puts(" max_drain=");
    bsp_uart_put_dec(stats.max_drain);
    bsp_uart_puts(" wakeups=");
    bsp_uart_put_dec((uint32_t) rx_isr_wakeups);
    bsp_uart_puts(" ticks=");
    bsp_uart_put_dec(elapsed);
    bsp_uart_puts("\n");

    bsp_uart_rx_irq_disable();
    bsp_irq_line_disable(1U << BSP_IRQ_LINE_EXTERNAL);

    bsp_test_report(ok);

    for (;;)
    {
        bsp_cpu_halt();
    }
}

VOID tx_application_define(VOID *first_unused_memory) /* parasoft-suppress MISRAC2025-RULE_8_6-a "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_5_8-b "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_8_13-a "parameter type fixed by the prototype in tx_api.h" */
{
    static TX_THREAD rx_test_thread;
    static ULONG rx_test_stack[TEST_STACK_SIZE / sizeof(ULONG)];
    UINT status;

    (void) first_unused_memory;
    status = tx_semaphore_create(&rx_semaphore, "uart rx", 0U);
    if (status == TX_SUCCESS)
    {
        status = tx_thread_create(&rx_test_thread,
                                  "uart rx test",
                                  rx_test_entry,
                                  0U,
                                  rx_test_stack,
                                  TEST_STACK_SIZE,
                                  TEST_PRIORITY,
                                  TEST_PRIORITY,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
    }

    if (status != TX_SUCCESS)
    {
        bsp_test_report(false);
    }
}

int main(void) /* parasoft-suppress MISRAC2025-RULE_8_6-a "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_5_8-b "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_8_3-c "separate freestanding images, one analysis project" */
{
    bsp_uart_init();
    bsp_uart_puts("ThreadX UART receive interrupt test\n");
    tx_kernel_enter();
    bsp_test_report(false);
    return 0;
}
