/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 *
 * Derived from the Eclipse ThreadX RISC-V64/GNU port:
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026-present Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  PORT SPECIFIC C INFORMATION                            RELEASE        */
/*                                                                        */
/*    tx_port.h                                        LoongArch64/GNU    */
/*                                                           1.0.0        */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Data type definitions and port constants that make the ThreadX      */
/*    kernel function on LoongArch64 (LA64, LP64D ABI, PLV0).             */
/*                                                                        */
/*  REQUIREMENTS                                                          */
/*                                                                        */
/*    LA-REQ-001  Port data model                                         */
/*    LA-REQ-002  Interrupt posture control                               */
/*                                                                        */
/**************************************************************************/

#ifndef TX_PORT_H
#define TX_PORT_H

#if !defined(__loongarch64) || !defined(__loongarch_lp64)
#error "The ThreadX LoongArch64 port requires an LA64 target with an LP64 ABI."
#endif

#if !defined(__loongarch_double_float)
#error "The ThreadX LoongArch64 port requires the LP64D ABI (64-bit FPU)."
#endif

#if defined(__loongarch_sx) || defined(__loongarch_asx)
#error "The ThreadX LoongArch64 port does not preserve LSX/LASX state. Build with -msimd=none."
#endif


/* LoongArch control and status register numbers used by the port.  */

#define TX_LA64_CSR_CRMD                        0x0     /* Current mode            */
#define TX_LA64_CSR_PRMD                        0x1     /* Pre-exception mode      */
#define TX_LA64_CSR_EUEN                        0x2     /* Extended unit enable    */
#define TX_LA64_CSR_ECFG                        0x4     /* Exception configuration */
#define TX_LA64_CSR_ESTAT                       0x5     /* Exception status        */
#define TX_LA64_CSR_ERA                         0x6     /* Exception return addr   */
#define TX_LA64_CSR_EENTRY                      0xC     /* Exception entry base    */
#define TX_LA64_CSR_SAVE0                       0x30    /* Scratch 0               */
#define TX_LA64_CSR_SAVE1                       0x31    /* Scratch 1               */
#define TX_LA64_CSR_TCFG                        0x41    /* Timer configuration     */
#define TX_LA64_CSR_TVAL                        0x42    /* Timer value             */
#define TX_LA64_CSR_TICLR                       0x44    /* Timer interrupt clear   */

/* CRMD.IE is bit 2.  The interrupt posture saved by TX_DISABLE is the
   previous value of this bit only.  */
#define TX_LA64_CRMD_IE                         0x4

/* Fixed context frame shared by exception entry, scheduling, and initial
   thread construction.  The 75 used 64-bit slots are rounded to 16-byte
   stack alignment.  */
#define TX_LA64_CONTEXT_FRAME_SIZE              608
#define TX_LA64_CONTEXT_ERA_OFFSET              256
#define TX_LA64_CONTEXT_PRMD_OFFSET             264
#define TX_LA64_CONTEXT_FPR_OFFSET              272
#define TX_LA64_CONTEXT_FCSR_OFFSET             528
#define TX_LA64_CONTEXT_FCC_OFFSET              536
#define TX_LA64_CONTEXT_INTERRUPT_TYPE          1
#define TX_LA64_SOLICITED_FRAME_SIZE             192
#define TX_LA64_SOLICITED_FPR_OFFSET             104
#define TX_LA64_SOLICITED_FCSR_OFFSET            168
#define TX_LA64_SOLICITED_PRMD_OFFSET             176


#ifndef __ASSEMBLER__

/* Include for memset.  */
#include <string.h>
#ifdef TX_INCLUDE_USER_DEFINE_FILE
#include "tx_user.h"
#endif /* TX_INCLUDE_USER_DEFINE_FILE */

#endif /* __ASSEMBLER__ */


/* Define ThreadX basic types for this port.  */

#define VOID                                    void

/* LONG and ULONG are 32-bit, as on every other ThreadX 64-bit port.  The
   kernel control-block layouts and queue message sizes depend on it.  Use
   ULONG64 for 64-bit quantities.  */

#ifndef __ASSEMBLER__
typedef char                                    CHAR;
typedef unsigned char                           UCHAR; /* parasoft-suppress MISRAC2025-DIR_4_6-a "DEV-003" */ /* parasoft-suppress MISRAC2025-DIR_4_6-c "DEV-003" */
typedef int                                     INT; /* parasoft-suppress MISRAC2025-DIR_4_6-a "DEV-003" */ /* parasoft-suppress MISRAC2025-DIR_4_6-c "DEV-003" */
typedef unsigned int                            UINT; /* parasoft-suppress MISRAC2025-DIR_4_6-a "DEV-003" */ /* parasoft-suppress MISRAC2025-DIR_4_6-c "DEV-003" */
typedef int                                     LONG; /* parasoft-suppress MISRAC2025-DIR_4_6-a "DEV-003" */ /* parasoft-suppress MISRAC2025-DIR_4_6-c "DEV-003" */
typedef unsigned int                            ULONG; /* parasoft-suppress MISRAC2025-DIR_4_6-a "DEV-003" */ /* parasoft-suppress MISRAC2025-DIR_4_6-c "DEV-003" */
typedef unsigned long long                      ULONG64; /* parasoft-suppress MISRAC2025-DIR_4_6-c "DEV-003" */
typedef short                                   SHORT; /* parasoft-suppress MISRAC2025-DIR_4_6-a "DEV-003" */ /* parasoft-suppress MISRAC2025-DIR_4_6-c "DEV-003" */
typedef unsigned short                          USHORT; /* parasoft-suppress MISRAC2025-DIR_4_6-a "DEV-003" */ /* parasoft-suppress MISRAC2025-DIR_4_6-c "DEV-003" */
#define ULONG64_DEFINED
#define ALIGN_TYPE_DEFINED
typedef unsigned long long                      ALIGN_TYPE; /* parasoft-suppress MISRAC2025-DIR_4_6-a "DEV-003" */ /* parasoft-suppress MISRAC2025-DIR_4_6-c "DEV-003" */
typedef char tx_la64_pointer_width_check[(sizeof(VOID *) == 8U) ? 1 : -1];
#endif /* __ASSEMBLER__ */

/* ULONG is 32-bit but pointers are 64-bit, so the thread pointer for a
   timeout is kept in a pointer-sized timer extension field.  */
#define TX_TIMER_INTERNAL_EXTENSION             VOID    *tx_timer_internal_extension_ptr;
#define TX_TIMER_EXTENSION_PTR_DEFINED

#define TX_THREAD_CREATE_TIMEOUT_SETUP(t)       (t) -> tx_thread_timer.tx_timer_internal_timeout_function =  &(_tx_thread_timeout);             /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */ \
                                                (t) -> tx_thread_timer.tx_timer_internal_timeout_param =     0;                                \
                                                (t) -> tx_thread_timer.tx_timer_internal_extension_ptr =     (VOID *) (t);

#define TX_THREAD_TIMEOUT_POINTER_SETUP(t)      TX_PARAMETER_NOT_USED(timeout_input);                                                          /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */ \
                                                (t) =  (TX_THREAD *) _tx_timer_expired_timer_ptr -> tx_timer_internal_extension_ptr;


/* Priority levels: 32 to 1024, evenly divisible by 32.  */

#ifndef TX_MAX_PRIORITIES
#define TX_MAX_PRIORITIES                       32
#endif


/* Minimum thread stack.  Covers the interrupt frame (integer and FP
   registers) plus a margin for the thread's own call chain.  */

#ifndef TX_MINIMUM_STACK
#define TX_MINIMUM_STACK                        1024
#endif


/* System timer thread stack size and priority.  Only applicable if
   TX_TIMER_PROCESS_IN_ISR is not defined.  */

#ifndef TX_TIMER_THREAD_STACK_SIZE
#define TX_TIMER_THREAD_STACK_SIZE              1024
#endif

#ifndef TX_TIMER_THREAD_PRIORITY
#define TX_TIMER_THREAD_PRIORITY                0
#endif


/* System tick rate.  Must match the value _tx_initialize_low_level
   programs into the periodic timer (LA-REQ-102).  Applications use it
   to convert tx_time_get() ticks to seconds.  */

#ifndef TX_TIMER_TICKS_PER_SECOND
#define TX_TIMER_TICKS_PER_SECOND               100
#endif


/* Interrupt posture values (CRMD.IE).  */

#define TX_INT_DISABLE                          0x00000000U
#define TX_INT_ENABLE                           ((UINT) TX_LA64_CRMD_IE)


/* Trace time stamp source.  The BSP may override these with the stable
   counter.  */

#ifndef TX_TRACE_TIME_SOURCE
#define TX_TRACE_TIME_SOURCE                    ++_tx_trace_simulated_time
#endif
#ifndef TX_TRACE_TIME_MASK
#define TX_TRACE_TIME_MASK                      0xFFFFFFFFUL
#endif


/* Port specific options for the _tx_build_options variable.  */

#define TX_PORT_SPECIFIC_BUILD_OPTIONS          0


/* In-line initialization constant.  */

#define TX_INLINE_INITIALIZATION


/* Stack checking forces stack filling.  */

#ifdef TX_ENABLE_STACK_CHECKING
#undef TX_DISABLE_STACK_FILLING /* parasoft-suppress MISRAC2025-RULE_20_5-a "DEV-012" */
#endif


/* Thread control block extensions.  */

#define TX_THREAD_EXTENSION_0
#define TX_THREAD_EXTENSION_1
#define TX_THREAD_EXTENSION_2
#define TX_THREAD_EXTENSION_3


/* Extensions of the remaining ThreadX objects.  */

#define TX_BLOCK_POOL_EXTENSION
#define TX_BYTE_POOL_EXTENSION
#define TX_EVENT_FLAGS_GROUP_EXTENSION
#define TX_MUTEX_EXTENSION
#define TX_QUEUE_EXTENSION
#define TX_SEMAPHORE_EXTENSION
#define TX_TIMER_EXTENSION


#ifndef TX_THREAD_USER_EXTENSION
#define TX_THREAD_USER_EXTENSION
#endif


#define TX_THREAD_CREATE_EXTENSION(thread_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */
#define TX_THREAD_DELETE_EXTENSION(thread_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */
#define TX_THREAD_COMPLETED_EXTENSION(thread_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */
#define TX_THREAD_TERMINATED_EXTENSION(thread_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */

#define TX_BLOCK_POOL_CREATE_EXTENSION(pool_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */
#define TX_BYTE_POOL_CREATE_EXTENSION(pool_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */
#define TX_EVENT_FLAGS_GROUP_CREATE_EXTENSION(group_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */ /* parasoft-suppress MISRAC2025-RULE_1_1-c "DEV-005" */
#define TX_MUTEX_CREATE_EXTENSION(mutex_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */
#define TX_QUEUE_CREATE_EXTENSION(queue_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */
#define TX_SEMAPHORE_CREATE_EXTENSION(semaphore_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */
#define TX_TIMER_CREATE_EXTENSION(timer_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */

#define TX_BLOCK_POOL_DELETE_EXTENSION(pool_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */
#define TX_BYTE_POOL_DELETE_EXTENSION(pool_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */
#define TX_EVENT_FLAGS_GROUP_DELETE_EXTENSION(group_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */ /* parasoft-suppress MISRAC2025-RULE_1_1-c "DEV-005" */
#define TX_MUTEX_DELETE_EXTENSION(mutex_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */
#define TX_QUEUE_DELETE_EXTENSION(queue_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */
#define TX_SEMAPHORE_DELETE_EXTENSION(semaphore_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */
#define TX_TIMER_DELETE_EXTENSION(timer_ptr) /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */


/* Interrupt lockout and restore.  All CSR access is done in assembly
   (tx_thread_interrupt_control.S), so no inline assembly appears in C code
   (MISRA C:2025 Directive 4.3).  */

#ifndef __ASSEMBLER__
UINT                                            _tx_thread_interrupt_control(UINT new_posture); /* parasoft-suppress MISRAC2025-RULE_5_10-a "DEV-002" */ /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-006" */

/* System tick entry point, called by the exception vector assembly.  The
   prototype is visible in every translation unit that includes this header,
   including the definition (Rule 8.4).  */
VOID                                            _tx_timer_interrupt(VOID); /* parasoft-suppress MISRAC2025-RULE_5_10-a "DEV-002" */
#endif

#define TX_INTERRUPT_SAVE_AREA                  UINT interrupt_save;

#define TX_DISABLE                              interrupt_save =  _tx_thread_interrupt_control(TX_INT_DISABLE);
#define TX_RESTORE                              (VOID) _tx_thread_interrupt_control(interrupt_save);


#define TX_BLOCK_POOL_DISABLE                   TX_DISABLE
#define TX_BYTE_POOL_DISABLE                    TX_DISABLE
#define TX_EVENT_FLAGS_GROUP_DISABLE            TX_DISABLE
#define TX_MUTEX_DISABLE                        TX_DISABLE
#define TX_QUEUE_DISABLE                        TX_DISABLE
#define TX_SEMAPHORE_DISABLE                    TX_DISABLE


/* Coverage test extensions for the ThreadX regression test.  The upstream
   regression suite and the port hook macros below are compiled only into
   regression builds (TX_REGRESSION_TEST is defined by
   test/regression/CMakeLists.txt).  */

#ifndef __ASSEMBLER__

#ifdef TX_REGRESSION_TEST

typedef unsigned int    TEST_FLAG; /* parasoft-suppress MISRAC2025-DIR_4_6-a "DEV-003" */ /* parasoft-suppress MISRAC2025-DIR_4_6-c "DEV-003" */
extern TEST_FLAG        threadx_byte_allocate_loop_test;
extern TEST_FLAG        threadx_byte_release_loop_test;
extern TEST_FLAG        threadx_mutex_suspension_put_test; /* parasoft-suppress MISRAC2025-RULE_1_1-a "DEV-005" */ /* parasoft-suppress MISRAC2025-RULE_1_1-b "DEV-005" */
extern TEST_FLAG        threadx_mutex_suspension_priority_test; /* parasoft-suppress MISRAC2025-RULE_1_1-a "DEV-005" */ /* parasoft-suppress MISRAC2025-RULE_1_1-b "DEV-005" */
#ifndef TX_TIMER_PROCESS_IN_ISR
extern TEST_FLAG        threadx_delete_timer_thread;
#endif

extern void             abort_and_resume_byte_allocating_thread(void); /* parasoft-suppress MISRAC2025-RULE_1_1-a "DEV-005" */ /* parasoft-suppress MISRAC2025-RULE_1_1-b "DEV-005" */
extern void             abort_all_threads_suspended_on_mutex(void); /* parasoft-suppress MISRAC2025-RULE_1_1-a "DEV-005" */ /* parasoft-suppress MISRAC2025-RULE_1_1-b "DEV-005" */
extern void             suspend_lowest_priority(void);
#ifndef TX_TIMER_PROCESS_IN_ISR
extern void             delete_timer_thread(void);
#endif
extern TEST_FLAG        test_stack_analyze_flag;
extern TEST_FLAG        test_initialize_flag;
extern TEST_FLAG        test_forced_mutex_timeout;

/* Upstream regression-suite hook (testcontrol.c defines it): dispatched
   from _tx_timer_interrupt before any tick processing.  The reference is
   weak, so builds that provide no dispatch function link and run without
   it (tx_timer_interrupt.c).  */
extern void             test_interrupt_dispatch(void) __attribute__((weak));




#define TX_BYTE_ALLOCATE_EXTENSION              if (threadx_byte_allocate_loop_test == ((TEST_FLAG) 1))         \
                                                {                                                               \
                                                    pool_ptr -> tx_byte_pool_owner =  TX_NULL;                  \
                                                    threadx_byte_allocate_loop_test = ((TEST_FLAG) 0);          \
                                                }

#define TX_BYTE_RELEASE_EXTENSION               if (threadx_byte_release_loop_test == ((TEST_FLAG) 1))          \
                                                {                                                               \
                                                    threadx_byte_release_loop_test = ((TEST_FLAG) 0);           \
                                                    abort_and_resume_byte_allocating_thread();                  \
                                                }

#define TX_MUTEX_PUT_EXTENSION_1                if (threadx_mutex_suspension_put_test == ((TEST_FLAG) 1))       \
                                                {                                                               \
                                                    threadx_mutex_suspension_put_test = ((TEST_FLAG) 0);        \
                                                    abort_all_threads_suspended_on_mutex();                     \
                                                }

#define TX_MUTEX_PUT_EXTENSION_2                if (test_forced_mutex_timeout == ((TEST_FLAG) 1))               \
                                                {                                                               \
                                                    test_forced_mutex_timeout = ((TEST_FLAG) 0);                \
                                                    _tx_thread_wait_abort(mutex_ptr -> tx_mutex_suspension_list); \
                                                }

#define TX_MUTEX_PRIORITY_CHANGE_EXTENSION      if (threadx_mutex_suspension_priority_test == ((TEST_FLAG) 1))   /* parasoft-suppress MISRAC2025-RULE_1_1-c "DEV-005" */ \
                                                {                                                               \
                                                    threadx_mutex_suspension_priority_test = ((TEST_FLAG) 0);   \
                                                    suspend_lowest_priority();                                  \
                                                }

#ifndef TX_TIMER_PROCESS_IN_ISR

#define TX_TIMER_INITIALIZE_EXTENSION(a)        if (threadx_delete_timer_thread == ((TEST_FLAG) 1))              /* parasoft-suppress MISRAC2025-DIR_4_9-a "DEV-004" */ \
                                                {                                                               \
                                                    threadx_delete_timer_thread = ((TEST_FLAG) 0);              \
                                                    delete_timer_thread();                                      \
                                                    (a) =  ((UINT) 1);                                          \
                                                }

#endif

#define TX_THREAD_STACK_ANALYZE_EXTENSION       if (test_stack_analyze_flag == ((TEST_FLAG) 1))                  /* parasoft-suppress MISRAC2025-RULE_1_1-c "DEV-005" */ \
                                                {                                                               \
                                                    thread_ptr -> tx_thread_id =  ((TEST_FLAG) 0);              \
                                                    test_stack_analyze_flag =     ((TEST_FLAG) 0);              \
                                                }                                                               \
                                                else if (test_stack_analyze_flag == ((TEST_FLAG) 2))            \
                                                {                                                               \
                                                    stack_ptr =  thread_ptr -> tx_thread_stack_start;           \
                                                    test_stack_analyze_flag =     ((TEST_FLAG) 0);              \
                                                }                                                               \
                                                else if (test_stack_analyze_flag == ((TEST_FLAG) 3))            \
                                                {                                                               \
                                                    *stack_ptr =  TX_STACK_FILL;                                \
                                                    test_stack_analyze_flag =     ((TEST_FLAG) 0);              \
                                                }                                                               \
                                                else                                                            \
                                                {                                                               \
                                                    test_stack_analyze_flag =     ((TEST_FLAG) 0);              \
                                                }

#define TX_INITIALIZE_KERNEL_ENTER_EXTENSION    if (test_initialize_flag == ((TEST_FLAG) 1))                     /* parasoft-suppress MISRAC2025-RULE_1_1-c "DEV-005" */ \
                                                {                                                               \
                                                    test_initialize_flag =  ((TEST_FLAG) 0);                    \
                                                    return;                                                     \
                                                }

#endif /* TX_REGRESSION_TEST */

#endif /* __ASSEMBLER__ */


/* ThreadX version ID.  The upstream regression suite prints this string
   (its testcontrol.c defines TX_THREAD_INIT before including tx_api.h, so
   that translation unit owns the single definition).  The declaration is
   always visible so a compatible declaration precedes the definition
   (Rule 8.4), and both state the array size explicitly (Rule 8.11).  */

#ifndef __ASSEMBLER__
extern  CHAR                    _tx_version_id[160]; /* parasoft-suppress MISRAC2025-RULE_5_10-a "DEV-002" */ /* parasoft-suppress MISRAC2025-RULE_8_6-b "DEV-013" */ /* parasoft-suppress MISRAC2025-RULE_2_8-a "DEV-013" */
#ifdef TX_THREAD_INIT
CHAR                            _tx_version_id[160] =
                                    "(c) 2026 Engineering Software Lab Israel (ESL). Based on Eclipse ThreadX. * ThreadX LoongArch64/GNU Version 6.5.2.202603 *"; /* parasoft-suppress MISRAC2025-RULE_5_10-a "DEV-002" */
#endif /* TX_THREAD_INIT */
#endif /* __ASSEMBLER__ */

#endif /* TX_PORT_H */
