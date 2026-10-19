/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/* void bringup_trigger_break(void): raise a BRK exception (Ecode 0xC).  */

    .text
    .globl  bringup_trigger_break
    .type   bringup_trigger_break, @function
bringup_trigger_break:
    break   0
    jr      $ra
    .size   bringup_trigger_break, . - bringup_trigger_break
