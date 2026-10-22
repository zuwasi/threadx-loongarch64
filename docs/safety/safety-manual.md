# Safety Manual: ThreadX Port for LoongArch64

| | |
|---|---|
| Document ID | ESL-TXLA64-SM |
| Revision | r1, 2026-10-22 (release 1.0.0-rc1) |
| Standard | IEC 61508-3:2010, Annex D (safety manual for compliant items) |
| Element | `ports/loongarch64/gnu/` and `bsp/qemu_virt/`, used with Eclipse ThreadX `v6.5.2.202603_rel` |

> **Status.** This is a release candidate. Required approvals and the
> independent functional safety assessment are pending, and structural
> coverage is reported as line coverage only. The element is **not
> certified** and must not be used in a safety function on the basis of
> this release alone.

## 1. Element description

The element is a software element out of context. It adapts Eclipse
ThreadX to LA64 processors (single core, PLV0, LP64D) and provides a
reference board support package for the QEMU `virt` machine. Its
functions are given in the [requirements](requirements.md) (LA-REQ-001 to
LA-REQ-012 for the port, LA-REQ-101 to LA-REQ-107 for the BSP), and its
design in [design.md](design.md).

## 2. Assumptions of use

The integrator must confirm that LA-AOU-001 to LA-AOU-006
([requirements.md](requirements.md), section 2) hold in their system.
In addition:

| ID | Constraint |
|---|---|
| SM-001 | Build with the exact options in `cmake/loongarch64_gnu.cmake`. In particular `-msimd=none`: the port does not save LSX/LASX state. |
| SM-002 | Use only the ThreadX submodule revision `v6.5.2.202603_rel`. A kernel update requires the regression suite and the compile-time offset checks (`tx_la64_port_assert.c`) to pass again. |
| SM-003 | Interrupt handlers run with interrupts disabled and must not enable them. Nesting is not supported (LA-REQ-009). |
| SM-004 | Every interrupt source other than the timer (ESTAT.IS[11]) and the BSP external line (IS[2]) is treated as fatal. A new source needs a port change and a re-verification. |
| SM-005 | Size each thread stack for its own call chain plus one 608-byte interrupt frame. `TX_MINIMUM_STACK` is 1024 bytes. Enable `TX_ENABLE_STACK_CHECKING` during integration. |
| SM-006 | Set the system stack (`link.ld`, 16 KiB) for the deepest handler call chain. The guard below it is checked only at test report time. Add a periodic check if the application needs one. |
| SM-007 | The tick rate is `TX_TIMER_TICKS_PER_SECOND` (default 100). `_tx_initialize_low_level` stops the system if the timer cannot be programmed. |
| SM-008 | The QEMU BSP is a reference. A BSP for real hardware must add the `.data` copy (LA-REQ-101), check the interrupt controller addresses, and be verified on that hardware. |

## 3. Failure behaviour

| Condition | Behaviour |
|---|---|
| Unexpected exception or interrupt | `bsp_fatal_exception`: masks all lines, prints Ecode, ESTAT, ERA and BADV, and powers off or halts (LA-REQ-106) |
| Timer initialisation failure | Fatal handler with zeroed arguments |
| Interrupt route read-back mismatch | `bsp_external_irq_init` returns `false`; the caller must not enable the line |
| UART receive ring full or line error | Byte dropped or error counted (`bsp_uart_rx_stats_get`); the handler continues |
| System stack overflow into the guard | Reported by `bsp_test_report` |

## 4. Verification evidence

| Evidence | Location | Result (1.0.0-rc1) |
|---|---|---|
| ThreadX regression suite, 6 configurations | `docs/verification/regression.md` | 576/576 passed |
| Bring-up and demo tests on QEMU | `test/bringup/`, `test/threadx_demo/` | 7/7 passed, Debug and Release |
| MISRA C:2025, product scope | `docs/misra/reports/` | 0 findings in port and BSP that are not covered by a deviation |
| Structural coverage | `docs/verification/coverage.md` | 92% line coverage (184/200); gaps justified; statement, branch and MC/DC open |
| Traceability | `docs/safety/traceability.md` | Every requirement linked |
| Summary | `docs/verification/verification-report.md` | |

## 5. Known limitations and open items

1. Statement, branch and MC/DC coverage are not yet reported (C/C++test
   Standard reports line coverage only). To be closed before an audit.
2. Real-hardware validation (planned candidate: Loongson 2K1000LA) has not
   been done. All results come from QEMU 8.2.2.
3. The deviation records DEV-001 to DEV-015 are drafts. DEV-001 is
   conditionally approved by the safety manager.
4. Independent code and design reviews are not yet recorded
   (`docs/reviews/review-log.md`).
5. The ThreadX common kernel is not MISRA C:2025 compliant (613 findings,
   `docs/misra/kernel-gap-analysis.md`). The integrator must justify its
   use as a pre-existing element.
