/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Bring-up test entry points (shared by bringup_main.c and
    bringup_fatal.c).

    bringup_trap_entry (bringup_trap.S) and bringup_trigger_break
    (bringup_break.S) are defined in assembly, so no C definition exists
    (Rule 8.6-b, asm-defined entries, as in bsp.h; DEV-006).  */

#ifndef BRINGUP_H
#define BRINGUP_H

#include <stdint.h>

void bringup_trap_entry(void); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */
void bringup_trigger_break(void); /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */

/* C exception handler called by bringup_trap.S.  */
void bringup_trap_handler(uint64_t estat);

#endif /* BRINGUP_H */
