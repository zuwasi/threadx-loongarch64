/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Power off and test result reporting.

    Requirement: LA-REQ-107  */

#include "bsp.h"

#ifdef CPPTEST_COVERAGE
#include "cpptest/cpptest.h"
#endif

void bsp_poweroff(void)
{
#ifdef CPPTEST_COVERAGE
    CppTest_FinalizeRuntime();
#endif

    volatile uint8_t *sleep_ctl = (volatile uint8_t *) BSP_GED_SLEEP_CTL_ADDR; /* parasoft-suppress MISRAC2025-RULE_11_4-b "DEV-007" */

    *sleep_ctl = (uint8_t) BSP_GED_SLEEP_CTL_POWEROFF;

    /* Not reached on QEMU.  On other platforms stop safely.  */
    bsp_cpu_halt();
}

void bsp_test_report(bool passed) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    bool stack_guard_ok = bsp_stack_guard_check();
    bool ok = passed;

    if (!stack_guard_ok)
    {
        bsp_uart_puts("System stack guard damaged\n");
        ok = false;
    }

    if (ok)
    {
        bsp_uart_puts("\n*** TEST PASSED ***\n");
    }
    else
    {
        bsp_uart_puts("\n*** TEST FAILED ***\n");
    }
    bsp_poweroff();
}
