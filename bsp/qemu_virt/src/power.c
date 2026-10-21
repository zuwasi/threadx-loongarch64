/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Power off and test result reporting.

    Requirement: LA-REQ-107  */

#include "bsp.h"

void bsp_poweroff(void)
{
    volatile uint8_t *sleep_ctl = (volatile uint8_t *) BSP_GED_SLEEP_CTL_ADDR; /* parasoft-suppress MISRAC2025-RULE_11_4-b "DEV-007" */

    *sleep_ctl = (uint8_t) BSP_GED_SLEEP_CTL_POWEROFF;

    /* Not reached on QEMU.  On other platforms stop safely.  */
    bsp_cpu_halt();
}

void bsp_test_report(bool passed) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    if (passed)
    {
        bsp_uart_puts("\n*** TEST PASSED ***\n");
    }
    else
    {
        bsp_uart_puts("\n*** TEST FAILED ***\n");
    }
    bsp_poweroff();
}
