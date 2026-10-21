/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/* ThreadX system tick processing for the QEMU virt timer interrupt.
   The timer register is acknowledged by the exception vector before entry.
   Requirements: LA-REQ-007, LA-REQ-009, LA-REQ-011, LA-REQ-102, LA-REQ-103. */

#include "tx_api.h"
#include "tx_timer.h"
#include "tx_thread.h"

#ifdef TX_REGRESSION_TEST
/* Upstream regression-suite hook, declared in the regression support
   block of tx_port.h: dispatch the per-test ISR before any tick
   processing, the same ordering the upstream CMake harnesses use.  The
   reference is weak, so builds that provide no dispatch function link
   and run without it.  Compiled only when TX_REGRESSION_TEST is defined;
   production kernels are unaffected (see docs/verification/regression.md).  */
#endif

VOID _tx_timer_interrupt(VOID) /* parasoft-suppress MISRAC2025-RULE_5_10-a "DEV-002" */
{
    TX_TIMER_INTERNAL **current_timer_slot;

#ifdef TX_REGRESSION_TEST
    if (&test_interrupt_dispatch != NULL)
    {
        test_interrupt_dispatch();
    }
#endif

    _tx_timer_system_clock = _tx_timer_system_clock + 1U;

    if (_tx_timer_time_slice != 0U)
    {
        _tx_timer_time_slice = _tx_timer_time_slice - 1U;
        if (_tx_timer_time_slice == 0U)
        {
            _tx_timer_expired_time_slice = TX_TRUE;
        }
    }

    current_timer_slot = _tx_timer_current_ptr;
    if (*current_timer_slot != TX_NULL)
    {
        _tx_timer_expired = TX_TRUE;
    }
    else
    {
        current_timer_slot = &current_timer_slot[1];
        if (current_timer_slot == _tx_timer_list_end)
        {
            current_timer_slot = _tx_timer_list_start;
        }
        _tx_timer_current_ptr = current_timer_slot;
    }

    if (_tx_timer_expired != TX_FALSE)
    {
        _tx_timer_expiration_process();
    }

    if (_tx_timer_expired_time_slice != TX_FALSE)
    {
        _tx_thread_time_slice();
    }
}
