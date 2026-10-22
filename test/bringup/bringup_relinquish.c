/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/* M6 integration test: repeatedly relinquish from threads with interrupts
   enabled while timer interrupts remain active.

   Verifies: LA-REQ-005, LA-REQ-009  */

#include "tx_api.h"
#include "bsp.h"

#define TEST_THREAD_COUNT  2U
#define TEST_STACK_SIZE    2048U
#define TEST_PRIORITY      5U
#define TEST_ITERATIONS    10000U
#define CRMD_IE_MASK       0x4ULL

static volatile ULONG relinquish_completed[TEST_THREAD_COUNT];

static VOID relinquish_test_entry(ULONG thread_input)
{
    static volatile ULONG posture_failures;
    ULONG iteration = 0U;
    ULONG done0;
    ULONG done1;
    UINT thread_index = (UINT) thread_input;

    while (iteration < TEST_ITERATIONS)
    {
        if ((bsp_crmd_read() & CRMD_IE_MASK) == 0U)
        {
            posture_failures = posture_failures + 1U;
        }
        (void) tx_thread_relinquish();
        iteration++;
    }

    relinquish_completed[thread_index] = 1U;
    done0 = relinquish_completed[0];
    done1 = relinquish_completed[1];
    if ((done0 != 0U) && (done1 != 0U))
    {
        bsp_test_report(posture_failures == 0U);
    }

    for (;;)
    {
        (void) tx_thread_sleep(1U);
    }
}

VOID tx_application_define(VOID *first_unused_memory) /* parasoft-suppress MISRAC2025-RULE_8_6-a "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_5_8-b "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_8_13-a "parameter type fixed by the prototype in tx_api.h" */
{
    static TX_THREAD relinquish_threads[TEST_THREAD_COUNT];
    static ULONG relinquish_stacks[TEST_THREAD_COUNT][TEST_STACK_SIZE / sizeof(ULONG)];
    UINT index;
    UINT status = TX_SUCCESS;

    (void) first_unused_memory;
    for (index = 0U; index < TEST_THREAD_COUNT; index++)
    {
        relinquish_completed[index] = 0U;
        status = tx_thread_create(&relinquish_threads[index],
                                  "relinquish",
                                  relinquish_test_entry,
                                  (ULONG) index,
                                  relinquish_stacks[index],
                                  TEST_STACK_SIZE,
                                  TEST_PRIORITY,
                                  TEST_PRIORITY,
                                  TX_NO_TIME_SLICE,
                                  TX_AUTO_START);
        if (status != TX_SUCCESS)
        {
            break;
        }
    }

    if (status != TX_SUCCESS)
    {
        bsp_test_report(false);
    }
}

int main(void) /* parasoft-suppress MISRAC2025-RULE_8_6-a "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_5_8-b "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_8_3-c "separate freestanding images, one analysis project" */
{
    bsp_uart_init();
    bsp_uart_puts("ThreadX interrupt-enabled relinquish test\n");
    tx_kernel_enter();
    bsp_test_report(false);
    return 0;
}
