/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/* Compiling this translation unit verifies the C/assembly ABI assumptions
   encoded in tx_port.h against the pinned ThreadX control block.
   Requirements: LA-REQ-001, LA-REQ-003, LA-REQ-005, LA-REQ-006. */

#include "tx_api.h"
#include "tx_thread.h"
#include "tx_port.h"

typedef char tx_la64_thread_run_count_offset_check[ /* parasoft-suppress MISRAC2025-RULE_2_3-b "DEV-010" */
    (__builtin_offsetof(TX_THREAD, tx_thread_run_count) == 4U) ? 1 : -1];
typedef char tx_la64_thread_stack_ptr_offset_check[ /* parasoft-suppress MISRAC2025-RULE_2_3-b "DEV-010" */
    (__builtin_offsetof(TX_THREAD, tx_thread_stack_ptr) == 8U) ? 1 : -1];
typedef char tx_la64_thread_stack_end_offset_check[ /* parasoft-suppress MISRAC2025-RULE_2_3-b "DEV-010" */
    (__builtin_offsetof(TX_THREAD, tx_thread_stack_end) == 24U) ? 1 : -1];
typedef char tx_la64_thread_time_slice_offset_check[ /* parasoft-suppress MISRAC2025-RULE_2_3-b "DEV-010" */
    (__builtin_offsetof(TX_THREAD, tx_thread_time_slice) == 36U) ? 1 : -1];
typedef char tx_la64_context_alignment_check[ /* parasoft-suppress MISRAC2025-RULE_2_3-b "DEV-010" */
    ((TX_LA64_CONTEXT_FRAME_SIZE % 16) == 0) ? 1 : -1];
