# Verification Report, Release 1.0.0-rc1

| | |
|---|---|
| Document ID | ESL-TXLA64-SVR |
| Revision | r1, 2026-10-22 |
| Plan | [docs/safety/verification.md](../safety/verification.md) |
| Baseline | Tag `v1.0.0-rc1` |
| Environment | WSL Ubuntu 24.04; GCC 14.2.0 `loongarch64-linux-gnu`; QEMU 8.2.2 `-M virt -cpu la464 -smp 1`; Parasoft C/C++test Standard 2026.1.3 |

## 1. Results against the verification plan

| Activity | Pass criterion | Result | Status |
|---|---|---|---|
| MISRA C:2025 analysis, product scope | No unjustified violation in port or BSP | 613 findings, all in `external/threadx/common`; 0 in port and BSP | Met (deviation approvals pending) |
| MISRA C:2025 analysis, test scope | Reported separately | 8665 findings: 8575 upstream, 90 in the regression-harness libc (accepted, `docs/misra/reports/README.md`), 0 in bring-up and demo (DEV-015 covers 2 tick reads) | Recorded |
| Flow analysis | No unjustified finding | Included in the MISRA configuration; DEV-014 | Met (approval pending) |
| Unit and bring-up tests | All pass | 7/7, Debug and Release | Met |
| Integration: ThreadX regression suite | 100% in 6 configurations | 96/96 in each, 576/576 | Met |
| Structural coverage | Line coverage with justified gaps for rc1 (statement, branch and MC/DC open) | 184/200 lines (92%); every gap justified in `coverage.md` | Partially met, open item |
| Tool validation (Parasoft) | All seeded violations found | 8/8 (2026-10-20) | Met |
| Code review | All findings closed | Not yet recorded | Open |
| Design review | All findings closed | Not yet recorded | Open |
| Traceability | Every requirement linked | `tools/trace.py --check` passes | Met |

## 2. Tests per requirement

See [traceability.md](../safety/traceability.md). New in this release:
`bringup_uart_rx_interrupts` (LA-REQ-105) sends a 90-byte payload into
QEMU's serial input and checks the following:

- the route was initialised and read back correctly;
- ECFG.LIE bit 2 is set;
- the ISR ran, with CRMD.IE = 0;
- every byte arrived intact;
- there was no overflow and no line error;
- the ISR woke the receiving thread.

Observed: 90 ISR calls for 90 bytes, in 3 ticks.

## 3. Defects found and fixed

| ID | Summary | Verification |
|---|---|---|
| PR-M4-001 | System stack leak when an interrupt is taken in the idle loop | Regression suite; `bringup_long_idle` |
| PR-M5-001 | Thread context captured on the system stack in `_tx_thread_system_return` | 600/600 stress run (M5); `bringup_relinquish_interrupts_enabled` |

## 4. Open items

1. Statement, branch and MC/DC coverage, before an assessment.
2. Independent code and design reviews.
3. Approval of the deviations DEV-001 to DEV-015 and of the documents listed
   in [approval-package.md](../approval-package.md).
4. Validation on real LoongArch hardware.

## 5. Conclusion

The release candidate meets the plan's test and static-analysis criteria
on QEMU. It does not yet meet the review, approval and full structural
coverage criteria, so it is released as a **pre-release** only.

| Role | Name | Date | Decision |
|---|---|---|---|
| Safety manager | Daniel Liezrowice, Managing Partner & Founder, ESL | | Pending |
