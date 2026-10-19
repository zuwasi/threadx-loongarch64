# Software Safety Requirements Specification

| | |
|---|---|
| Document ID | ESL-TXLA64-SRS |
| Revision | r1, 2026-10-19 (baseline for M2) |
| Related | [Safety plan](safety-plan.md), [Verification plan](verification.md) |

## 1. Conventions

- Requirement IDs are `LA-REQ-nnn` and never reused.
- Source files and tests cite the IDs they implement or verify.
- Verification methods: **T** test, **A** analysis, **R** review, **I** inspection.

## 2. Assumptions of use

The port is a software element out of context. The integrator must confirm
these assumptions:

| ID | Assumption |
|---|---|
| LA-AOU-001 | The processor is LA64 with a 64-bit FPU and executes at privilege level PLV0. |
| LA-AOU-002 | Single hardware thread. Other cores are held in reset or do not access ThreadX data. |
| LA-AOU-003 | The application does not use LSX/LASX vector instructions. |
| LA-AOU-004 | Code and data are in memory that is not modified by other bus masters during execution. |
| LA-AOU-005 | Hardware faults (memory errors, clock failure) are detected by hardware or system-level mechanisms outside this element. |
| LA-AOU-006 | The integrator performs the system hazard analysis and sets the tick rate, stack sizes and priorities. |

## 3. Port requirements

| ID | Requirement | Verification |
|---|---|---|
| LA-REQ-001 | The port shall define the ThreadX data types so that `LONG` and `ULONG` are 32 bits, `ULONG64` and `ALIGN_TYPE` are 64 bits, and pointers are 64 bits (LP64). | A, T |
| LA-REQ-002 | `_tx_thread_interrupt_control` shall set CRMD.IE to the requested posture and return the previous CRMD.IE value, as one atomic operation. | T, R |
| LA-REQ-003 | On an interrupt, the port shall save the complete integer register context (r1-r31 except r0), ERA, PRMD and the FP context (f0-f31, FCSR0, FCC0-7) of the interrupted thread. | T, R |
| LA-REQ-004 | On return from an interrupt, the port shall restore exactly the context saved by LA-REQ-003 for the thread selected by the scheduler. | T, R |
| LA-REQ-005 | A thread that suspends voluntarily shall save only the callee-saved registers (r1, r3, r22-r31, f24-f31, FCSR0) required by the LP64D calling convention. | T, R |
| LA-REQ-006 | `_tx_thread_stack_build` shall create an initial frame so that the first dispatch of a thread starts `_tx_thread_shell_entry` with interrupts enabled and a 16-byte aligned stack pointer. | T, A |
| LA-REQ-007 | The scheduler shall wait with interrupts enabled when no thread is ready, and dispatch the highest-priority ready thread. | T |
| LA-REQ-008 | The stack pointer shall be 16-byte aligned at every call from port assembly into C. | A, R |
| LA-REQ-009 | Interrupt nesting is not supported. Interrupts shall remain disabled from exception entry until context restore. | T, R |
| LA-REQ-010 | The port shall provide `memcpy`, `memmove`, `memset` and `memcmp` for the freestanding build, with ISO C99 semantics. | T, A |
| LA-REQ-011 | `_tx_initialize_low_level` shall set the exception entry (EENTRY), save the system stack pointer and the first free memory address, and configure the system tick. | T |
| LA-REQ-012 | The port shall enable the FPU (EUEN.FPE) before any FP context is saved or restored. | T, R |

## 4. BSP requirements (QEMU `virt`)

| ID | Requirement | Verification |
|---|---|---|
| LA-REQ-101 | The startup code shall set up the stack, clear `.bss`, initialise `.data` and call `main` before any C object is used. | T, R |
| LA-REQ-102 | The BSP shall generate the ThreadX tick from the LoongArch constant timer at a configurable rate (default 100 Hz). | T |
| LA-REQ-103 | The timer interrupt shall be acknowledged through TICLR before the kernel timer function is called. | T, R |
| LA-REQ-104 | The BSP shall provide polled character output on the NS16550 UART for console and test reports. | T |
| LA-REQ-105 | The BSP shall route the UART receive interrupt through the platform interrupt controllers to a CPU interrupt line. | T |
| LA-REQ-106 | Any exception that is not an expected interrupt shall call a fatal error handler that disables interrupts, reports the exception code and ERA on the console, and stops execution. | T |
| LA-REQ-107 | The BSP shall provide a test exit mechanism so the regression suite reports pass or fail to the host. | T |

## 5. Approval

| Role | Name | Date | Decision |
|---|---|---|---|
| Technical lead | Daniel Liezrowice, Managing Partner & Founder, ESL | 2026-10-19 | Approved as M2 baseline |
| Safety manager | Pending | | |
