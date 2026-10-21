/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/* Eight equal-priority threads sleep on the hardware tick, retain a live
   double across the system-return path, and complete a bounded run.
   Verifies LA-REQ-002 through LA-REQ-009 and LA-REQ-011/012,
   plus LA-REQ-102/103/107. */

#include "tx_api.h"
#include "tx_timer.h"
#include "bsp.h"

#define DEMO_THREAD_COUNT  8U
#define DEMO_STACK_SIZE    2048U
#define DEMO_ITERATIONS    32U
#define DEMO_SPIN_TICKS    16U
#define DEMO_PRIORITY      5U

static volatile ULONG demo_completed[DEMO_THREAD_COUNT];
static volatile ULONG demo_spin_ready[DEMO_THREAD_COUNT];

static VOID demo_fail(VOID)
{
    bsp_uart_puts("ThreadX demo failure\n");
    bsp_test_report(false);
    bsp_poweroff();
    for (;;)
    {
        bsp_cpu_halt();
    }
}

static bool demo_all_completed(VOID)
{
    UINT index;
    bool complete = true;

    for (index = 0U; index < DEMO_THREAD_COUNT; index++)
    {
        if (demo_completed[index] == 0U)
        {
            complete = false;
        }
    }

    return complete;
}

static bool demo_all_spin_ready(VOID)
{
    UINT index;
    bool ready = true;

    for (index = 0U; index < DEMO_THREAD_COUNT; index++)
    {
        if (demo_spin_ready[index] == 0U)
        {
            ready = false;
        }
    }

    return ready;
}

static VOID demo_thread_entry(ULONG thread_input)
{
    ULONG iteration = 0U;
    ULONG spin_count = 0U;
    ULONG start_tick;
    double fp_state = 0.0;
    double expected;

    if (thread_input >= DEMO_THREAD_COUNT)
    {
        demo_fail();
    }

    while (iteration < DEMO_ITERATIONS)
    {
        (void) tx_thread_sleep(1U);
        fp_state += 0.25;
        expected = ((double) iteration + 1.0) * 0.25;
        if (fp_state != expected) /* parasoft-suppress MISRAC2025-RULE_10_1-h "intentional exact-equality FP check; != also detects NaN corruption" */
        {
            demo_fail();
        }
        iteration++;
    }

    /* This barrier makes time-slice preemption necessary: every thread must
       reach the spin phase before any one can complete it. */
    demo_spin_ready[thread_input] = 1U;
    while (!demo_all_spin_ready())
    {
        fp_state += 0.125;
        spin_count++;
    }

    /* Keep this thread runnable so the one-tick round-robin slice forces
       asynchronous interrupt preemption while floating state is live. */
    start_tick = _tx_timer_system_clock;
    while ((_tx_timer_system_clock - start_tick) < DEMO_SPIN_TICKS)
    {
        fp_state += 0.125;
        spin_count++;
    }
    expected = ((double) DEMO_ITERATIONS * 0.25) +
               ((double) spin_count * 0.125);
    if (fp_state != expected) /* parasoft-suppress MISRAC2025-RULE_10_1-h "intentional exact-equality FP check; != also detects NaN corruption" */
    {
        demo_fail();
    }

    demo_completed[thread_input] = 1U;
    if (thread_input == 0U)
    {
        while (!demo_all_completed())
        {
            (void) tx_thread_sleep(1U);
        }

        bsp_uart_puts("ThreadX demo: 8 threads, tick, and FP context PASS\n");
        bsp_test_report(true);
        bsp_poweroff();
        for (;;)
        {
            bsp_cpu_halt();
        }
    }

    for (;;)
    {
        (void) tx_thread_sleep(1U);
    }
}

VOID tx_application_define(VOID *first_unused_memory) /* parasoft-suppress MISRAC2025-RULE_8_6-a "upstream testcontrol.c defines the same entry; separate images" */ /* parasoft-suppress MISRAC2025-RULE_5_8-b "upstream testcontrol.c defines the same entry; separate images" */ /* parasoft-suppress MISRAC2025-RULE_8_13-a "parameter type fixed by the prototype in tx_api.h" */
{
    UINT index;
    UINT status;

    static TX_THREAD demo_threads[DEMO_THREAD_COUNT];
    static ULONG demo_stacks[DEMO_THREAD_COUNT][DEMO_STACK_SIZE / sizeof(ULONG)];

    (void) first_unused_memory;
    bsp_uart_puts("Starting ThreadX LoongArch64 demo\n");

    for (index = 0U; index < DEMO_THREAD_COUNT; index++)
    {
        demo_completed[index] = 0U;
        demo_spin_ready[index] = 0U;
        status = tx_thread_create(&demo_threads[index],
                                  "demo",
                                  demo_thread_entry,
                                  (ULONG) index,
                                  demo_stacks[index],
                                  DEMO_STACK_SIZE,
                                  DEMO_PRIORITY,
                                  DEMO_PRIORITY,
                                  1U,
                                  TX_AUTO_START);
        if (status != TX_SUCCESS)
        {
            demo_fail();
        }
    }
}

int main(void) /* parasoft-suppress MISRAC2025-RULE_8_6-a "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_5_8-b "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_8_3-c "separate freestanding images, one analysis project" */
{
    bsp_uart_init();
    tx_kernel_enter();
    demo_fail();
    return 0;
}
