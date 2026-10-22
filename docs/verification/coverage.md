# Structural Coverage Report

| | |
|---|---|
| Document ID | ESL-TXLA64-COV |
| Revision | r1, 2026-10-21 |
| Tool | Parasoft C/C++test Standard 2026.1.3 (`cpptestcc` instrumentation, `cpptestcli` builtin://Coverage report) |
| Script | `tools/parasoft/coverage.sh <fresh-output-dir> [root\|all]` |

## 1. Scope and method

Instrumented code: all ESL-owned C translation units of the port
(`ports/loongarch64/gnu/`) and the BSP (`bsp/qemu_virt/`). Assembly is
verified through requirements-based tests and review, not structural
coverage (project plan, section 7).

`coverage.sh` builds every scope with `cpptestcc`, runs its tests on QEMU,
decodes the coverage frames the runtime writes on the UART
(`collect_coverage.py`) and generates one report per scope, each with its
own workspace. Running the scopes separately is needed because the
regression configurations redefine ThreadX `TX_*` macros, so the same
function (for example `_tx_timer_interrupt`) has several compiled variants.
Merging them into one report makes C/C++test flag conflicting data and
suppress coverage for that function.

## 2. Coverage metric for release 1.0.0-rc1

The instrumentation is built with statement, decision and MC/DC options
enabled. However, C/C++test Standard reports **line coverage only**.
Statement, branch and MC/DC reporting needs a different Parasoft product
(C/C++test CT or Professional) or a second coverage tool.

**Decision for 1.0.0-rc1:** line coverage is the reported structural metric.
Statement, branch and MC/DC coverage (IEC 61508-3 Table B.2, safety plan
Table A.5) is an **open item**. It must be closed before an assessment or
audit. Until then, this release makes no claim that statement, branch or
MC/DC coverage targets have been met.

## 3. Results, root scope (bring-up tests and demo, 7/7 tests passed)

| File | Covered lines | Line coverage |
|---|---|---|
| `bsp/qemu_virt/src/fatal.c` | 11/11 | 100% |
| `bsp/qemu_virt/src/intr.c` | 25/30 | 83% |
| `bsp/qemu_virt/src/power.c` | 7/13 | 54% |
| `bsp/qemu_virt/src/timer.c` | 23/23 | 100% |
| `bsp/qemu_virt/src/uart.c` | 72/76 | 95% |
| `ports/loongarch64/gnu/libc/src/tx_la64_string.c` | 30/30 | 100% |
| `ports/loongarch64/gnu/src/tx_timer_interrupt.c` | 16/17 | 94% |
| **Total** | **184/200** | **92%** |

## 4. Uncovered lines and justification

| Location | Lines | Reason | Class |
|---|---|---|---|
| `power.c` `bsp_poweroff` | 22, 24, 27 | Executed on every run, but after `CppTest_FinalizeRuntime()` has flushed the coverage data, so they cannot be recorded. Every passing test proves line 24 ran, because QEMU exits only through the poweroff write. Line 27 cannot be reached on QEMU (defensive halt). | Measurement limit / defensive |
| `power.c` `bsp_test_report` | 37, 38, 47 | Failure-reporting paths (stack guard damaged, test failed). These run only when a test fails, and all tests pass. | Defensive / failure path |
| `intr.c` `bsp_external_irq_init` | 70, 75, 80, 85, 90 | Read-back verification failures of the interrupt route. QEMU always returns the programmed values. These lines are reached only if a controller is faulty or behaves differently. | Defensive |
| `uart.c` `bsp_uart_putc` | 58 | Empty busy-wait body. QEMU's transmitter is always ready (THRE set). | Platform-dependent |
| `uart.c` `bsp_uart_rx_isr` | 149, 163 | UART line error and receive-ring overflow. QEMU's serial input produces neither. | Defensive / error path |
| `uart.c` `bsp_uart_rx_read` | 196 | The early exit when the ring empties before `length` bytes. The test reads exactly the number of available bytes. | Test gap (planned unit test) |
| `tx_timer_interrupt.c` `_tx_timer_interrupt` | 55 | Timer-list wrap-around. Not reached in the short root-scope tests. The ThreadX regression suite (scope `all`, one report per configuration) runs long enough to wrap the timer list. | Covered by regression scope |

## 5. Open items

1. Statement, branch and MC/DC coverage of port and BSP C code (see section 2).
2. Fault-injection tests for the defensive paths in section 4, or reviewed
   justifications approved by the technical lead.
3. A unit test for the `bsp_uart_rx_read` early exit.

## 6. Reproduction

```sh
export CPPTEST_SETTINGS=<local settings file with the license>
tools/parasoft/coverage.sh build/parasoft-coverage-<id> root   # or: all
```

Reports: `<out>/report-<scope>/report.html` and `coverage.xml`.
