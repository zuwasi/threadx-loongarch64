# Project Plan: Eclipse ThreadX Port for LoongArch64

## 1. Objective

Deliver a production-quality Eclipse ThreadX port for the LoongArch64 (LA64)
architecture, with a board support package (BSP) for the QEMU `virt` machine.
The port and BSP are developed following an IEC 61508-3 software process
targeting Systematic Capability 3 (SC 3, for SIL 3 applications). All C code is
written to MISRA C:2025 and verified with Parasoft C/C++test.

This project does not claim IEC 61508 certification. It produces the process
evidence that a certification effort would build on.

## 2. Scope

### Release 1.0 (this plan)

| Item | Content |
|---|---|
| Architecture port | LA64, single core, GNU toolchain, LP64D ABI |
| BSP | QEMU `loongarch64` `virt`: startup, linker script, stable-counter timer tick, NS16550 UART, interrupt controller path for external interrupts |
| Verification | Full ThreadX regression suite passing on QEMU; port/BSP unit tests; structural coverage |
| Static analysis | MISRA C:2025 (Parasoft C/C++test 2026.1), zero undocumented violations in port and BSP |
| Gap analysis | MISRA C:2025 report for ThreadX common kernel code (`external/threadx/common`), report only |

### Out of scope for 1.0

- SMP (ThreadX SMP port)
- LA32R / LA32S
- Real hardware (planned candidate: Loongson 2K1000LA)
- ThreadX Modules (user/kernel separation)

## 3. Baseline

| Component | Version |
|---|---|
| Eclipse ThreadX | `v6.5.2.202603_rel`, git submodule in `external/threadx` |
| Compiler | GCC 14 `loongarch64-linux-gnu`, used freestanding (`-ffreestanding -nostdlib`) |
| Emulator | QEMU 8.2 `qemu-system-loongarch64 -M virt` |
| Static analysis | Parasoft C/C++test Standard 2026.1.3, built-in "MISRA C 2025" configuration |
| Build | CMake + Ninja |

ThreadX sources are never modified in the submodule. Any required change is
proposed upstream or carried as a documented patch.

## 4. Repository layout

```
external/threadx/             Eclipse ThreadX (submodule, pinned tag)
ports/loongarch64/gnu/
    inc/tx_port.h             Port definitions
    src/*.S, *.c              Context switch, scheduler, ISR entry, timer
bsp/qemu_virt/                Startup, linker script, UART, timer, irq controller
test/
    regression/               ThreadX regression suite integration (CTest + QEMU)
    unit/                     Port and BSP unit tests
cmake/                        Toolchain file(s)
docs/
    safety/                   Safety plan, requirements, design, verification
    misra/                    Guideline enforcement plan, deviations, reports
tools/                        Scripts: build, run on QEMU, Parasoft analysis
```

## 5. Technical approach

The RISC-V64 GNU port in ThreadX is the structural template. It already has a
64-bit context frame, CSR-based trap handling, no branch delay slots, and a
QEMU-based regression BSP (`test/tx/cmake/riscv`). The MIPS32 port is used only
as a reference for interrupt nesting behaviour.

Key LoongArch elements:

| Concern | LoongArch mechanism |
|---|---|
| Trap entry / return | `EENTRY`, `ERA`, `PRMD`, `CRMD`, `ertn` |
| Interrupt enable | `CRMD.IE`, `ECFG.LIE` (per-line enable) |
| Pending cause | `ESTAT.Ecode`, `ESTAT.IS` |
| System tick | Constant timer: `TCFG`, `TVAL`, `TICLR` |
| Scratch for trap entry | `SAVE0`..`SAVE3` CSRs |
| External interrupts (virt) | PCH-PIC -> EIOINTC -> CPU interrupt line |

Port files to implement:

- `tx_initialize_low_level.S`
- `tx_thread_context_save.S`, `tx_thread_context_restore.S`
- `tx_thread_schedule.S`, `tx_thread_system_return.S`
- `tx_thread_stack_build.S`
- `tx_thread_interrupt_control.S`
- `tx_timer_interrupt.c` (C where possible, to maximise MISRA coverage)

Inline assembly in C is confined to dedicated wrapper functions/macros in
`tx_port.h` (MISRA C:2025 Directive 4.3).

## 6. Process and evidence (IEC 61508-3, SC 3)

| Document | Location | IEC 61508 reference |
|---|---|---|
| Software Safety Plan | `docs/safety/safety-plan.md` | Part 1 cl. 6, Part 3 cl. 6 |
| Software Safety Requirements Specification | `docs/safety/requirements.md` | Part 3 cl. 7.2 |
| Software Architecture and Design | `docs/safety/design.md` | Part 3 cl. 7.4 |
| Coding standard and Guideline Enforcement Plan | `docs/misra/gep.md` | Part 3 Table A.3, MISRA Compliance:2020 |
| Tool classification and validation (T1/T2/T3) | `docs/safety/tools.md` | Part 3 cl. 7.4.4 |
| Verification plan and report | `docs/safety/verification.md` | Part 3 cl. 7.9 |
| Traceability matrix | `docs/safety/traceability.md` | Part 3 cl. 7.4.2.12 |
| Port safety manual | `docs/safety/safety-manual.md` | Part 3 Annex D |
| MISRA compliance summary and deviation records | `docs/misra/` | MISRA Compliance:2020 |

Requirement IDs (`LA-REQ-xxx`) are referenced from design sections, source
file headers, and tests, so traceability can be generated rather than kept by
hand.

## 7. Verification strategy

1. **Static analysis:** Parasoft MISRA C:2025 configuration on all port and BSP
   C code. Every remaining finding has a deviation record. Analysis runs on each
   change; HTML and SARIF reports are committed to `docs/misra/reports/`.
2. **Regression:** the ThreadX regression suite runs on QEMU through CTest.
   Release 1.0 requires a 100% pass rate in default, optimised (`-O2`) and trace
   configurations.
3. **Unit tests:** port and BSP C functions (timer setup, interrupt controller
   routing, UART driver) tested in isolation.
4. **Structural coverage:** statement and branch coverage of port and BSP C
   code; MC/DC for decisions with multiple conditions (IEC 61508-3 Table B.2,
   SC 3). Assembly is covered through requirements-based tests and review.
5. **Reviews:** design and code reviews recorded against a checklist.

## 8. Milestones

| ID | Milestone | Exit criteria |
|---|---|---|
| M0 | Project setup | Repo, submodule, toolchain, CMake skeleton, CI builds |
| M1 | Plans and requirements | Safety plan, GEP, tool classification, requirements baseline |
| M2 | Boot and console on QEMU | BSP boots, UART output, trap handler, timer interrupt fires |
| M3 | Port bring-up | ThreadX demo (8-thread sample) runs on QEMU |
| M4 | Regression suite | All regression tests pass in all configurations, in CI |
| M5 | MISRA C:2025 compliance | Port + BSP: zero undocumented violations; reports committed |
| M6 | Unit tests and coverage | Coverage targets met or justified |
| M7 | Kernel gap analysis | MISRA C:2025 report on ThreadX common code |
| M8 | Release 1.0 | Safety manual, traceability, release notes, tag `v1.0.0` |

## 9. Continuous integration

GitHub Actions on Ubuntu 24.04: install GCC 14 for LoongArch and QEMU, build
all configurations, and run the regression suite under QEMU. Parasoft analysis
runs on a licensed workstation; its reports are committed with each analysed
baseline.

## 10. Risks

| Risk | Mitigation |
|---|---|
| Parasoft has no built-in compiler definition for the LoongArch cross-GCC | Create a custom compiler configuration; validate it on known-violation samples |
| QEMU timing differs from silicon | Timing-dependent tests use tick counts, not wall time; real-board phase later |
| Toolchain intended for Linux targets | Freestanding flags, own startup and linker script; documented in tool classification |
| Kernel code not compliant with MISRA C:2025 | Out of scope to fix; reported in gap analysis |
