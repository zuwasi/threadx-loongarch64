# Release Notes

## 1.0.0-rc1 (2026-10-22), pre-release

The first release candidate of the Eclipse ThreadX port for LoongArch64
with the QEMU `virt` BSP.

**Not a certified release.** Approvals, independent reviews, full
structural coverage and real-hardware validation are still open (see
"Known limitations").

### Contents

- LA64 port (`ports/loongarch64/gnu/`): context save and restore with full
  integer and FP state, solicited and interrupt frames, scheduler with
  `idle`, atomic interrupt posture control, stack build, low-level
  initialisation, tick processing in C, and freestanding memory functions.
- QEMU `virt` BSP (`bsp/qemu_virt/`): startup with a system stack guard, a
  constant-timer tick, an NS16550 console with polled transmit and
  interrupt-driven receive, the PCH-PIC to EXTIOI to CPU interrupt route
  (new: LA-REQ-105), a fatal exception handler, and a test exit.
- Tests: six bring-up images (timer and posture, fatal exception, memory
  functions, long idle, relinquish with interrupts enabled, UART receive
  interrupt), the eight-thread demo, and the upstream ThreadX regression
  suite (96 tests, 6 configurations).
- Evidence: safety plan, requirements, design, compiler behaviour, tool
  classification, MISRA C:2025 enforcement plan, re-categorisation plan,
  15 deviation records, compliance summary and dashboard, kernel gap
  analysis, generated traceability matrix, coverage report, verification
  report and safety manual.

### Changes since the M5 baseline

- UART receive interrupt through PCH-PIC and EXTIOI; exception entry
  dispatches ESTAT.IS[2] (LA-REQ-105).
- M6 bring-up tests and the system stack guard.
- Parasoft coverage per scope (`tools/parasoft/coverage.sh`).
- `tools/run_qemu.sh --rx-file/--rx-after` to feed serial input to a test.
- Requirements traceability generator (`tools/trace.py`).
- DEV-015, and IOCSR accessors added to DEV-006.

### Verification summary

Regression 576/576; bring-up and demo 7/7 (Debug and Release); MISRA
C:2025: 0 port or BSP findings that are not covered by a deviation; line
coverage 92%. Details are in `docs/verification/verification-report.md`.

### Known limitations

- Single core only; no LSX/LASX; no interrupt nesting.
- Results come from QEMU 8.2.2 only.
- Coverage is line coverage only; MC/DC will be added before an audit.
- Deviation records, documents and reviews are awaiting approval.
