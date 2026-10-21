/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Regression support layer.  The build is freestanding, so this file is
    the C library as far as the upstream suite is concerned: suite
    completion, wall-clock time, and rand/srand.  */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bsp.h"
#include "tx_api.h"

/* Suite completion (testcontrol.c, EXTERNAL_EXIT).  Report the result in
   the format tools/run_qemu.sh judges, then end the QEMU run through the
   BSP poweroff path (LA-REQ-107).  */
void external_exit(unsigned int status)
{
    if (status == 0U)
    {
        (void) printf("\n*** TEST PASSED ***\n");
    }
    else
    {
        (void) printf("\n*** TEST FAILED *** (%u errors)\n", status);
    }

    bsp_poweroff();

    /* bsp_poweroff ends the QEMU run; halt defensively if it ever
       returns.  */
    for (;;)
    {
        bsp_cpu_halt();
    }
}

/* Kernel-setup test exit path.  */
void exit(int status)
{
    external_exit((unsigned int) status);
}

/* Wall clock from the ThreadX tick counter.  The tests use time() only to
   bound run durations (LA-REQ-102).  */
time_t time(time_t *timer)
{
    time_t seconds;

    seconds = (time_t) (tx_time_get() / (ULONG) TX_TIMER_TICKS_PER_SECOND);

    if (timer != NULL)
    {
        *timer = seconds;
    }

    return seconds;
}

/* Minimal deterministic LCG (same parameters as the upstream RISC-V
   harness).  Only threadx_thread_priority_change.c calls rand().  */
static unsigned long rand_seed = 1UL;

int rand(void)
{
    rand_seed = (rand_seed * 1103515245UL) + 12345UL;
    return (int) ((rand_seed >> 16) & 0x7FFFUL);
}

void srand(unsigned int seed)
{
    rand_seed = (unsigned long) seed;
}
