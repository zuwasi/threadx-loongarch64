/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Fatal exception handler.

    Called for any exception that the system does not expect.  Reports the
    cause and stops execution; the system does not attempt to continue.

    Requirement: LA-REQ-106  */

#include "bsp.h"

#define ESTAT_ECODE_SHIFT   16U
#define ESTAT_ECODE_MASK    0x3FULL

void bsp_fatal_exception(uint64_t estat, uint64_t era, uint64_t badv) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    bsp_irq_line_disable(0x1FFFU);

    bsp_uart_puts("\n*** FATAL EXCEPTION ***\nEcode=");
    bsp_uart_put_hex64((estat >> ESTAT_ECODE_SHIFT) & ESTAT_ECODE_MASK);
    bsp_uart_puts(" ESTAT=");
    bsp_uart_put_hex64(estat);
    bsp_uart_puts(" ERA=");
    bsp_uart_put_hex64(era);
    bsp_uart_puts(" BADV=");
    bsp_uart_put_hex64(badv);
    bsp_uart_puts("\n");

    bsp_poweroff();
}
