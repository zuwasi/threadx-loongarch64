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
typedef unsigned char                           UCHAR;
typedef int                                     INT;
typedef unsigned int                            UINT;
typedef int                                     LONG;
typedef unsigned int                            ULONG;
typedef unsigned long long                      ULONG64;
typedef short                                   SHORT;
typedef unsigned short                          USHORT;
#define ULONG64_DEFINED
#define ALIGN_TYPE_DEFINED
typedef unsigned long long                      ALIGN_TYPE;
#endif /* __ASSEMBLER__ */

/* ULONG is 32-bit but pointers are 64-bit, so the thread pointer for a
   timeout is kept in a pointer-sized timer extension field.  */
#define TX_TIMER_INTERNAL_EXTENSION             VOID    *tx_timer_internal_extension_ptr;
#define TX_TIMER_EXTENSION_PTR_DEFINED

#define TX_THREAD_CREATE_TIMEOUT_SETUP(t)       (t) -> tx_thread_timer.tx_timer_internal_timeout_function =  &(_tx_thread_timeout);            \
                                                (t) -> tx_thread_timer.tx_timer_internal_timeout_param =     0;                                \
                                                (t) -> tx_thread_timer.tx_timer_internal_extension_ptr =     (VOID *) (t);

#define TX_THREAD_TIMEOUT_POINTER_SETUP(t)      TX_PARAMETER_NOT_USED(timeout_input);                                                         \
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


/* Interrupt posture values (CRMD.IE).  */

#define TX_INT_DISABLE                          0x00000000
#define TX_INT_ENABLE                           TX_LA64_CRMD_IE


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
#undef TX_DISABLE_STACK_FILLING
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


#define TX_THREAD_CREATE_EXTENSION(thread_ptr)
#define TX_THREAD_DELETE_EXTENSION(thread_ptr)
#define TX_THREAD_COMPLETED_EXTENSION(thread_ptr)
#define TX_THREAD_TERMINATED_EXTENSION(thread_ptr)

#define TX_BLOCK_POOL_CREATE_EXTENSION(pool_ptr)
#define TX_BYTE_POOL_CREATE_EXTENSION(pool_ptr)
#define TX_EVENT_FLAGS_GROUP_CREATE_EXTENSION(group_ptr)
#define TX_MUTEX_CREATE_EXTENSION(mutex_ptr)
#define TX_QUEUE_CREATE_EXTENSION(queue_ptr)
#define TX_SEMAPHORE_CREATE_EXTENSION(semaphore_ptr)
#define TX_TIMER_CREATE_EXTENSION(timer_ptr)

#define TX_BLOCK_POOL_DELETE_EXTENSION(pool_ptr)
#define TX_BYTE_POOL_DELETE_EXTENSION(pool_ptr)
#define TX_EVENT_FLAGS_GROUP_DELETE_EXTENSION(group_ptr)
#define TX_MUTEX_DELETE_EXTENSION(mutex_ptr)
#define TX_QUEUE_DELETE_EXTENSION(queue_ptr)
#define TX_SEMAPHORE_DELETE_EXTENSION(semaphore_ptr)
#define TX_TIMER_DELETE_EXTENSION(timer_ptr)


/* Interrupt lockout and restore.  All CSR access is done in assembly
   (tx_thread_interrupt_control.S), so no inline assembly appears in C code
   (MISRA C:2025 Directive 4.3).  */

#ifndef __ASSEMBLER__
UINT                                            _tx_thread_interrupt_control(UINT new_posture);
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


/* Coverage test extensions for the ThreadX regression test.  */

#ifndef __ASSEMBLER__

typedef unsigned int    TEST_FLAG;
extern TEST_FLAG        threadx_byte_allocate_loop_test;
extern TEST_FLAG        threadx_byte_release_loop_test;
extern TEST_FLAG        threadx_mutex_suspension_put_test;
extern TEST_FLAG        threadx_mutex_suspension_priority_test;
#ifndef TX_TIMER_PROCESS_IN_ISR
extern TEST_FLAG        threadx_delete_timer_thread;
#endif

extern void             abort_and_resume_byte_allocating_thread(void);
extern void             abort_all_threads_suspended_on_mutex(void);
extern void             suspend_lowest_priority(void);
#ifndef TX_TIMER_PROCESS_IN_ISR
extern void             delete_timer_thread(void);
#endif
extern TEST_FLAG        test_stack_analyze_flag;
extern TEST_FLAG        test_initialize_flag;
extern TEST_FLAG        test_forced_mutex_timeout;


#ifdef TX_REGRESSION_TEST

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

#define TX_MUTEX_PRIORITY_CHANGE_EXTENSION      if (threadx_mutex_suspension_priority_test == ((TEST_FLAG) 1))  \
                                                {                                                               \
                                                    threadx_mutex_suspension_priority_test = ((TEST_FLAG) 0);   \
                                                    suspend_lowest_priority();                                  \
                                                }

#ifndef TX_TIMER_PROCESS_IN_ISR

#define TX_TIMER_INITIALIZE_EXTENSION(a)        if (threadx_delete_timer_thread == ((TEST_FLAG) 1))             \
                                                {                                                               \
                                                    threadx_delete_timer_thread = ((TEST_FLAG) 0);              \
                                                    delete_timer_thread();                                      \
                                                    (a) =  ((UINT) 1);                                          \
                                                }

#endif

#define TX_THREAD_STACK_ANALYZE_EXTENSION       if (test_stack_analyze_flag == ((TEST_FLAG) 1))                 \
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

#define TX_INITIALIZE_KERNEL_ENTER_EXTENSION    if (test_initialize_flag == ((TEST_FLAG) 1))                    \
                                                {                                                               \
                                                    test_initialize_flag =  ((TEST_FLAG) 0);                    \
                                                    return;                                                     \
                                                }

#endif /* TX_REGRESSION_TEST */

#endif /* __ASSEMBLER__ */


/* ThreadX version ID.  */

#ifndef __ASSEMBLER__
#ifdef TX_THREAD_INIT
CHAR                            _tx_version_id[] =
                                    "(c) 2026 Engineering Software Lab Israel (ESL). Based on Eclipse ThreadX. * ThreadX LoongArch64/GNU Version 6.5.2.202603 *";
#else
extern  CHAR                    _tx_version_id[];
#endif /* TX_THREAD_INIT */
#endif /* __ASSEMBLER__ */

#endif /* TX_PORT_H */
