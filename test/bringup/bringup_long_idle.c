/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/* M6 integration test: leave ThreadX idle for more than the 26 ticks that
   exposed the system-stack leak found in M4.

   Verifies: LA-REQ-007, LA-REQ-009, LA-REQ-011, LA-REQ-102, LA-REQ-103  */

#include "tx_api.h"
#include "bsp.h"

#define IDLE_TEST_TICKS  40U
#define TEST_STACK_SIZE  2048U
#define TEST_PRIORITY    5U

static VOID idle_test_entry(ULONG thread_input)
{
    ULONG start_tick;
    ULONG end_tick;
    ULONG elapsed_ticks;
    UINT status;

    (void) thread_input;
    start_tick = tx_time_get();
    status = tx_thread_sleep(IDLE_TEST_TICKS);
    end_tick = tx_time_get();
    elapsed_ticks = end_tick - start_tick;

    bsp_uart_puts("ThreadX idle interval ticks=");
    bsp_uart_put_dec((uint32_t) elapsed_ticks); /* parasoft-suppress MISRAC2025-DIR_4_7-a "DEV-015" */
    bsp_uart_puts("\n");
    bsp_test_report((status == TX_SUCCESS) && (elapsed_ticks >= IDLE_TEST_TICKS));

    for (;;)
    {
        bsp_cpu_halt();
    }
}

VOID tx_application_define(VOID *first_unused_memory) /* parasoft-suppress MISRAC2025-RULE_8_6-a "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_5_8-b "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_8_13-a "parameter type fixed by the prototype in tx_api.h" */
{
    static TX_THREAD idle_test_thread;
    static ULONG idle_test_stack[TEST_STACK_SIZE / sizeof(ULONG)];
    UINT status;

    (void) first_unused_memory;
    status = tx_thread_create(&idle_test_thread,
                              "idle test",
                              idle_test_entry,
                              0U,
                              idle_test_stack,
                              TEST_STACK_SIZE,
                              TEST_PRIORITY,
                              TEST_PRIORITY,
                              TX_NO_TIME_SLICE,
                              TX_AUTO_START);
    if (status != TX_SUCCESS)
    {
        bsp_test_report(false);
    }
}

int main(void) /* parasoft-suppress MISRAC2025-RULE_8_6-a "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_5_8-b "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_8_3-c "separate freestanding images, one analysis project" */
{
    bsp_uart_init();
    bsp_uart_puts("ThreadX long-idle test\n");
    tx_kernel_enter();
    bsp_test_report(false);
    return 0;
}
