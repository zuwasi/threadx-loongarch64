/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  M2 test: an unexpected exception reaches the fatal handler.

    A BRK exception (Ecode 0xC) is raised with the BSP default entry
    installed.  The test passes if the console shows the fatal report with
    Ecode 0xC; reaching the end of main is a failure.

    Verifies: LA-REQ-106  */

#include "bsp.h"
#include "bringup.h"

int main(void) /* parasoft-suppress MISRAC2025-RULE_8_6-a "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_5_8-b "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_8_3-c "separate freestanding images, one analysis project" */
{
    bsp_uart_init();
    bsp_uart_puts("Fatal exception test: raising BRK\n");
    bringup_trigger_break();
    bsp_test_report(false);
    return 0;
}
