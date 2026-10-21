# ThreadX Regression Suite on QEMU (M4)

| | |
|---|---|
| Document ID | ESL-TXLA64-REG |
| Revision | r1, 2026-10-20 |
| Verification plan | `docs/safety/verification.md`, "Integration tests" |

## 1. Harness

The upstream Eclipse ThreadX regression suite (`external/threadx/test/tx/regression`)
runs unmodified on the LoongArch64 port and the QEMU `virt` BSP. The harness is
a standalone CMake project in `test/regression/`, modelled on the upstream
RISC-V QEMU harness (`test/tx/cmake/riscv`).

| Element | Implementation |
|---|---|
| Test selection | The 95 cases of the upstream RISC-V QEMU list (without its RISC-V-only FPU test) plus the standalone `threadx_initialize_kernel_setup_test`: 96 CTest tests, listed in `test/regression/CMakeLists.txt`. `threadx_trace_basic_test` is built in every configuration, as upstream does. |
| Execution model | One ELF per test (`CTEST`, `BATCH_TEST`), linked with `testcontrol.c`, the weak hook defaults, the BSP and the kernel |
| Completion | `EXTERNAL_EXIT`: `external_exit()` prints `*** TEST PASSED ***` or `*** TEST FAILED ***` and powers QEMU off; `tools/run_qemu.sh` judges the console |
| C library | Freestanding subset in `test/regression/libc`: `printf`, `puts`, `exit`, `external_exit`, `time` (from the tick counter), `rand`/`srand` |
| Timer ISR hook | `_tx_timer_interrupt` calls `test_interrupt_dispatch()` first, compiled only with `TX_REGRESSION_TEST` |
| Host macro | `-U__linux__`: the Linux-targeted cross compiler predefines it, which would select POSIX-only test code |

### Configurations

| Configuration | Kernel defines (all add `TX_QUEUE_MESSAGE_MAX_SIZE=32`) | Optimisation |
|---|---|---|
| `default` | none | `-O0` |
| `optimized` | none | `-O2` |
| `disable_notify_callbacks` | `TX_DISABLE_NOTIFY_CALLBACKS` | `-O0` |
| `stack_checking` | `TX_ENABLE_STACK_CHECKING` | `-O0` |
| `stack_checking_rand_fill` | `TX_ENABLE_STACK_CHECKING`, `TX_ENABLE_RANDOM_NUMBER_STACK_FILLING` | `-O0` |
| `trace` | `TX_ENABLE_EVENT_TRACE` | `-O0` |

### Running

```
tools/regression.sh <configuration>   # or: all
```

CI runs every configuration as a separate job (`.github/workflows/build.yml`,
job `regression`).

## 2. Results

Environment: QEMU 8.2.2 (`-M virt -cpu la464 -smp 1`), GCC 14.2.0
`loongarch64-linux-gnu`, ThreadX `v6.5.2.202603_rel`, local WSL Ubuntu 24.04.
Baseline: working tree after M3, before the M4 commit.

| Configuration | Tests | Passed | Failed |
|---|---|---|---|
| `default` | 96 | 96 | 0 |
| `optimized` | 96 | 96 | 0 |
| `disable_notify_callbacks` | 96 | 96 | 0 |
| `stack_checking` | 96 | 96 | 0 |
| `stack_checking_rand_fill` | 96 | 96 | 0 |
| `trace` | 96 | 96 | 0 |

The root project's bring-up and demo tests (3 tests) pass in Debug and Release
on the same baseline.

## 3. Defects found

### PR-M4-001: system stack leak on interrupts taken in the idle loop

| | |
|---|---|
| Severity | High: memory corruption, then loss of service |
| Component | `ports/loongarch64/gnu/src/tx_thread_context_save.S` |
| Requirements | LA-REQ-003, LA-REQ-009 |
| Status | Fixed; verified by the regression suite |

**Symptom.** In the first `default` run, 5 of 96 tests (`thread_multiple_sleep`,
`thread_sleep_for_100ticks`, `time_get_set`, `timer_large_timer_accuracy`,
`timer_multiple`) reported success and then hung in `test_control_cleanup`.

**Analysis.** QEMU interrupt tracing and monitor inspection showed
`_tx_byte_pool_created_ptr`, the last object in `.bss`, overwritten with an
address inside the system stack. The system stack held a chain of interrupt
frames whose ERA was the scheduler `idle` instruction.

When the timer interrupt arrives while the scheduler is idle, the vector
builds the 608-byte frame on the system stack. `_tx_thread_context_save` found
no current thread and kept that stack pointer; `_tx_thread_context_restore`
then re-entered the scheduler without releasing the frame. Each idle tick
leaked one frame, so about 26 consecutive idle ticks overflowed the 16 KiB
system stack into `.bss`. Only tests in which every thread sleeps for more
than about 26 ticks reached the overflow. The M3 demo never idles that long.

**Fix.** In `_tx_thread_context_save`, an interrupt that did not interrupt a
thread restarts on `_tx_thread_system_stack_ptr`, dropping the unused frame.
This is the behaviour of `_tx_thread_idle_system_save` in the upstream RISC-V
port.

**Follow-up.** The linker script places the system stack directly above
`.bss` with no guard. A stack guard region and a bring-up test that idles for
more than 26 ticks are candidates for M6.

### PR-M5-001: thread context captured on the system stack in `_tx_thread_system_return`

| | |
|---|---|
| Severity | High: the affected thread never runs again (loss of service) |
| Component | `ports/loongarch64/gnu/src/tx_thread_system_return.S` |
| Requirements | LA-REQ-005, LA-REQ-008, LA-REQ-009 |
| Status | Fixed; verified by a stress run and the regression suite |

**Symptom.** During M5 validation, single tests timed out in otherwise green
runs (`optimized::thread_wait_abort_and_isr`, `stack_checking::thread_simple_sleep`,
`default::byte_memory_information`), each passing in under 1 s when rerun
alone. Running `threadx_thread_wait_abort_and_isr_test` 600 times, 24 QEMU
instances in parallel, hung 231 runs.

**Analysis.** QEMU monitor inspection of hung runs showed the tick counter
still advancing and the scheduler dispatching the same thread repeatedly. The
thread's saved stack pointer was `0x2d70b0`, one 608-byte interrupt frame
below the system stack top, outside the thread's own stack; the restored ERA
pointed into `_tx_thread_schedule`. So the thread's "context" was an interrupt
frame taken in the scheduler.

`_tx_thread_system_return` assumed it is always called with interrupts
disabled. `_tx_thread_relinquish` (and other kernel paths) call it after
`TX_RESTORE`, with interrupts enabled. An interrupt between the switch to the
system stack and the clearing of `_tx_thread_current_ptr` made
`_tx_thread_context_save` store the system-stack frame as the current
thread's context. Host load widens the window, which is why it showed up as
rare timeouts.

**Fix.** `_tx_thread_system_return` records the caller's interrupt posture
and disables interrupts on entry, before any stack or pointer bookkeeping;
the solicited return restores the recorded posture. The upstream RISC-V64
port does the same (`csrci mstatus` at the start of its system return).

**Verification.** After the fix: the same 600-run, 24-way stress passes
600/600; the whole `default` suite repeated 4 times at `ctest -j 16` passes;
all six configurations pass 96/96; root Debug and Release pass 3/3.

**Follow-up.** A targeted unit test that calls `tx_thread_relinquish` with
interrupts enabled under a high tick rate is a candidate for M6.
## 4. Test-only code in the port

`tx_timer_interrupt.c` contains the regression ISR hook under
`#ifdef TX_REGRESSION_TEST`, and the hook is declared in the
`TX_REGRESSION_TEST` block of `tx_port.h`. Production builds do not define
that macro, so the hook, and its GCC `weak` attribute, are not part of the
delivered port. The MISRA C:2025 test-harness scope (`tools/misra.sh tests`)
analyses this code; the product compliance scope does not build it.
