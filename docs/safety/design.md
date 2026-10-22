# Software Architecture and Design Description

| | |
|---|---|
| Document ID | ESL-TXLA64-SAD |
| Revision | r1, 2026-10-22 (release 1.0.0-rc1) |
| Standard | IEC 61508-3:2010, 7.4.3 to 7.4.5 |
| Related | [Requirements](requirements.md), [Traceability](traceability.md), [Safety manual](safety-manual.md) |

## 1. Architecture

The element has three layers. Only the first two are delivered as safety
evidence. ThreadX itself is a pre-existing element that is used unmodified.

| Layer | Location | Language | Content |
|---|---|---|---|
| Port | `ports/loongarch64/gnu/` | Assembly, C99 | Context save and restore, scheduler, interrupt posture, stack build, low-level init, tick processing, freestanding memory functions |
| BSP (QEMU `virt`) | `bsp/qemu_virt/` | Assembly, C99 | Reset entry, linker script, timer, UART, external interrupt route, fatal handler, power-off and test exit |
| Kernel | `external/threadx/common/` (submodule, `v6.5.2.202603_rel`) | C | Eclipse ThreadX, not modified |

The interfaces are:

- **Kernel to port:** the macros and prototypes in `tx_port.h` (data types,
  `TX_DISABLE`/`TX_RESTORE`, extension hooks) and the port functions that
  the kernel calls (`_tx_thread_schedule`, `_tx_thread_system_return`,
  `_tx_thread_stack_build`, `_tx_thread_context_save` and
  `_tx_thread_context_restore`, `_tx_initialize_low_level`).
- **Port to BSP:** `bsp.h` (`bsp_timer_init`, `bsp_timer_ack`,
  `bsp_irq_line_enable`/`disable`, `bsp_external_irq_handler`,
  `bsp_fatal_exception`). Every CSR and IOCSR access is done in assembly
  (`bsp_cpu.S`, port `*.S`), so C code contains no inline assembly
  (MISRA C:2025 Dir 4.3).

## 2. Memory map and startup (LA-REQ-101, LA-REQ-012)

QEMU loads the ELF at its link address and starts `_start` (`startup.S`) at
PLV0 in direct-address mode. Because `.data` is already at its run address,
"initialise `.data`" means no copy is needed on this platform. A BSP for
ROM-based hardware must add the copy.

| Region | Placement (`link.ld`) |
|---|---|
| `.text` (with `.text.start` and 4 KiB-aligned trap entries) | from `0x00200000` |
| `.rodata`, `.data`, `.bss` | after `.text`; `.data` is page aligned |
| System stack guard | 256 bytes, filled with `0x5AA55AA55AA55AA5` |
| System stack | 16 KiB, grows down to the guard |
| Free memory | `__free_memory_start`, handed to `tx_application_define` |

`_start` disables interrupts, sets the stack, clears `.bss`, fills the
stack guard, enables the FPU (EUEN.FPE) and clears FCSR0. It then installs
a fatal default trap entry and calls `main`. `bsp_test_report` checks the
guard (`bsp_stack_guard_check`) before it reports a result.

## 3. Context frames (LA-REQ-003, LA-REQ-004, LA-REQ-005, LA-REQ-006)

| Frame | Size | Content | Built by |
|---|---|---|---|
| Interrupt frame (type 1) | 608 bytes | r1 to r31, ERA, PRMD, f0 to f31, FCSR0, FCC0 to FCC7 | Exception vector; `_tx_thread_stack_build` for a new thread |
| Solicited frame (type 0) | 192 bytes | ra, sp, r22 to r31, f24 to f31, FCSR0, the caller's CRMD posture | `_tx_thread_system_return` |

Slot 0 of each frame holds its type, and the scheduler restores by type.
`tx_la64_port_assert.c` checks the `TX_THREAD` field offsets that the
assembly uses (run count, stack pointer, stack end, time slice), and the
16-byte frame alignment, at compile time.

## 4. Exception and interrupt handling (LA-REQ-009, LA-REQ-103, LA-REQ-105, LA-REQ-106)

`_tx_thread_vector_entry` (4 KiB aligned, installed in EENTRY by
`_tx_initialize_low_level`) runs with CRMD.IE cleared by hardware:

1. It saves the stack pointer through SAVE0 and builds the interrupt frame
   on the interrupted stack.
2. It decodes ESTAT:
   - Ecode not zero: a synchronous exception, so it calls
     `bsp_fatal_exception`.
   - IS[2] set: the external interrupt, handled by
     `_tx_thread_context_save`, then `bsp_external_irq_handler`, then
     `_tx_thread_context_restore`.
   - IS[11] set: the timer, handled by `bsp_timer_ack` (TICLR), then
     `_tx_thread_context_save`, then `_tx_timer_interrupt`, then
     `_tx_thread_context_restore`.
   - Anything else: fatal.
3. Interrupts stay disabled until `ertn` in the restore path, so nesting is
   impossible (LA-REQ-009).

`_tx_thread_context_save` switches to the system stack when a thread was
interrupted. When the idle loop was interrupted, it drops the frame and
restarts on the system stack (fix for PR-M4-001).
`_tx_thread_context_restore` resumes the interrupted thread, or keeps the
frame as that thread's saved context and re-enters the scheduler when
preemption is due.

### UART receive route (LA-REQ-105)

`bsp_external_irq_init` (`intr.c`) programs the route that was verified
against the QEMU 8.2.2 sources:

```
NS16550 UART -> PCH-PIC input 2 (HTMSI vector 2, line unmasked)
            -> EXTIOI input 2 (enable bit 2, IPMAP group 0 -> IP0, COREMAP CPU 0)
            -> CPU interrupt pin 2 -> ESTAT.IS[2]
```

It reads every register back and returns `false` if any value differs.
The handler first clears the EXTIOI service latch (COREISR), then drains
the UART FIFO into a 128-byte single-producer, single-consumer ring
(`bsp_uart_rx_isr`), then calls an optional notification callback.
Clearing the latch first means a byte that arrives during the drain raises
the line again instead of being lost. Overflow and line errors are counted
and never block the handler.

## 5. Scheduling (LA-REQ-005, LA-REQ-007)

`_tx_thread_schedule` enables interrupts and waits with `idle 0` until
`_tx_thread_execute_ptr` is set. It then disables interrupts, commits the
thread, loads the time slice and restores the thread's frame by type.

`_tx_thread_system_return` records the caller's interrupt posture and
disables interrupts *before* any stack change (fix for PR-M5-001). It then
saves the solicited frame and enters the scheduler on the system stack.

## 6. Interrupt posture (LA-REQ-002)

`_tx_thread_interrupt_control` uses one `csrxchg` on CRMD.IE, so the read
and the write cannot be separated by an interrupt. `TX_DISABLE` and
`TX_RESTORE` in `tx_port.h` call it.

## 7. System tick (LA-REQ-011, LA-REQ-102)

`_tx_initialize_low_level` saves the system stack pointer and the first
free address, enables the FPU, installs the vector, disables all interrupt
lines, calls `bsp_timer_init(TX_TIMER_TICKS_PER_SECOND)` and enables line
11. `bsp_timer_init` derives the counter frequency from CPUCFG words 4 and
5 and programs a periodic reload. If the result is invalid, it calls the
fatal handler. `_tx_timer_interrupt` (C) performs the standard ThreadX
tick bookkeeping.

## 8. Freestanding library (LA-REQ-010)

`tx_la64_string.c` implements `memcpy`, `memmove`, `memset` and `memcmp`
with ISO C99 semantics, byte by byte. No other C library is linked
(`-nostdlib`; CI check `tools/check_freestanding_libc.sh`, DEV-001).

## 9. Design decisions

| Decision | Reason |
|---|---|
| Single interrupt frame layout for interrupts and new threads | One restore path; the first dispatch of a thread uses the same `ertn` code that has already been tested |
| No interrupt nesting | Simpler and fully analysable stack usage; worst-case latency is bounded by the longest handler |
| CSR access only in assembly | MISRA Dir 4.3; one audited place for each privileged instruction |
| Read-back verification of the interrupt route | Detects mis-programming or a controller that differs from the model |
| Fatal handler stops the system | Defensive behaviour required by LA-REQ-106; no attempt to continue in an unknown state |

## 10. Approval

| Role | Name | Date | Decision |
|---|---|---|---|
| Technical lead | Osnat Gropper, ESL | | Pending |
| Safety manager | Daniel Liezrowice, Managing Partner & Founder, ESL | | Pending |
