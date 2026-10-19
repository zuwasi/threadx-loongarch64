/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _tx_thread_interrupt_control                     LoongArch64/GNU    */
/*                                                           1.0.0        */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Sets CRMD.IE to the requested posture and returns the previous      */
/*    posture.  csrxchg reads and writes CRMD in one instruction, so the  */
/*    operation cannot be split by an interrupt.                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    new_posture                       TX_INT_ENABLE or TX_INT_DISABLE   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    old_posture                       Previous CRMD.IE value            */
/*                                                                        */
/*  REQUIREMENT                                                           */
/*                                                                        */
/*    LA-REQ-002                                                          */
/*                                                                        */
/**************************************************************************/

#include "tx_port.h"

    .text
    .globl  _tx_thread_interrupt_control
    .type   _tx_thread_interrupt_control, @function
_tx_thread_interrupt_control:
    li.w    $t0, TX_LA64_CRMD_IE
    and     $a0, $a0, $t0
    csrxchg $a0, $t0, TX_LA64_CSR_CRMD
    and     $a0, $a0, $t0
    jr      $ra
    .size   _tx_thread_interrupt_control, . - _tx_thread_interrupt_control
