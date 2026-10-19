# Software Safety Plan

| | |
|---|---|
| Document ID | ESL-TXLA64-SSP |
| Project | Eclipse ThreadX port for LoongArch64 |
| Standard | IEC 61508-3:2010, Systematic Capability 3 (SC 3) |
| Organisation | Engineering Software Lab Israel (ESL) |
| Revision | r1, 2026-10-19 |

## 1. Purpose and scope

This plan defines the activities, roles and evidence for developing the
LoongArch64 architecture port and the QEMU `virt` board support package (BSP)
of Eclipse ThreadX following IEC 61508-3 for SC 3.

The port is developed as a **software element out of context**. Its safety
requirements are assumptions about how an integrator will use it; they are
listed in the requirements specification and must be confirmed by the
integrator in their system context.

This project does not claim certification. It produces the evidence that a
certification by an assessment body would examine.

| In scope | Out of scope |
|---|---|
| Port: `ports/loongarch64/gnu/` | ThreadX common kernel (`external/threadx/common`), used as a pre-existing element |
| BSP: `bsp/qemu_virt/` | Hardware safety mechanisms of any LoongArch device |
| Tests, analysis and documents in this repository | System-level hazard analysis (integrator responsibility) |

## 2. Roles and independence

| Role | Person | Responsibility |
|---|---|---|
| Technical lead | Daniel Liezrowice, ESL | Design, implementation, review of technical content, Advisory deviation approval |
| Safety manager | To be appointed (ESL) | Owns this plan; approves Required deviations, verification reports and releases |
| Verifier | To be appointed per review | Reviews code and tests; must not be the author of the reviewed item |

IEC 61508-1 Table 5 requires, for SC 3, verification by an independent person
and a functional safety assessment by an independent department. The safety
manager and the technical lead are different people. The functional safety
assessment is outside the scope of this project and is left to the integrator
or an assessment body.

## 3. Lifecycle

The project follows the IEC 61508-3 V-model, adapted to a port element:

| Phase | IEC 61508-3 clause | Output | Milestone |
|---|---|---|---|
| Software safety requirements | 7.2 | [requirements.md](requirements.md) | M1 |
| Validation planning | 7.3 | [verification.md](verification.md) | M1 |
| Architecture and design | 7.4.2-7.4.5 | `design.md` | M2-M3 |
| Coding | 7.4.6-7.4.7 | Source code, MISRA evidence | M2-M5 |
| Module testing | 7.4.7 | Unit tests and coverage | M6 |
| Integration testing | 7.4.8 | ThreadX regression suite on QEMU | M4 |
| Validation | 7.7 | Verification report | M8 |
| Modification | 7.8 | Change process (section 6) | Ongoing |

## 4. Techniques and measures (SC 3)

Selected from IEC 61508-3 Annex A. HR = highly recommended.

| Table | Technique | Rating at SC 3 | Applied as |
|---|---|---|---|
| A.1 | Computer-aided specification tools | R | Markdown requirements with IDs, generated traceability |
| A.2 | Modular approach | HR | Port / BSP / libc separation; one function per file where practical |
| A.2 | Defensive programming | HR | Unexpected exceptions go to a fatal handler that stops the system |
| A.3 | Suitable programming language, strongly typed | HR | C99 with MISRA C:2025 |
| A.3 | Language subset | HR | MISRA C:2025, see [GEP](../misra/gep.md) |
| A.3 | Certified or increased-confidence tools | HR | See [tools.md](tools.md) |
| A.4 | Semi-formal methods | HR | State and sequence descriptions in the design |
| A.4 | Design and coding standards | HR | MISRA C:2025 and project coding rules |
| A.5 | Static analysis | HR | Parasoft C/C++test, MISRA C:2025 and flow analysis |
| A.5 | Dynamic analysis and testing | HR | Unit tests, ThreadX regression suite on QEMU |
| A.5 | Test coverage (statement, branch, MC/DC) | HR / HR / R | Statement and branch for all C; MC/DC for multi-condition decisions |
| A.9 | Static analysis, walk-through, design review | HR | Recorded reviews |
| A.10 | Functional safety assessment | HR | Out of scope (integrator / assessor) |

## 5. Configuration management

- All work products are in this git repository. Every release is a tag.
- The ThreadX baseline is a submodule pinned to `v6.5.2.202603_rel`. It is not
  modified; required changes are proposed upstream or kept as documented
  patches.
- Tool versions are recorded in [tools.md](tools.md). A tool version change is
  a change request.
- Approvals are recorded by name, role and date in the approved document and
  confirmed by a signed git tag.

## 6. Change management

Every change is made through a pull request that states:

1. the requirements affected;
2. the impact on design, MISRA compliance and tests;
3. the verification performed.

A change to a released element repeats the affected verification steps.

## 7. Work products

| Document | Location | Status |
|---|---|---|
| Software Safety Plan | `docs/safety/safety-plan.md` | r1 |
| Software Safety Requirements Specification | `docs/safety/requirements.md` | r1 |
| Tool classification | `docs/safety/tools.md` | r1 |
| Verification plan | `docs/safety/verification.md` | r1 |
| Design description | `docs/safety/design.md` | M2-M3 |
| MISRA Guideline Enforcement Plan | `docs/misra/gep.md` | r1 |
| MISRA Re-categorization Plan | `docs/misra/grp.md` | r1 |
| Deviation records | `docs/misra/deviations/` | Ongoing |
| Compiler behaviour (Dir 1.1) | `docs/safety/compiler-behaviour.md` | M5 |
| Traceability matrix | `docs/safety/traceability.md` | M8 |
| Safety manual | `docs/safety/safety-manual.md` | M8 |

## 8. Approval

| Role | Name | Date | Decision |
|---|---|---|---|
| Technical lead | Daniel Liezrowice, Managing Partner & Founder, ESL | 2026-10-19 | Approved |
| Safety manager | Pending | | |
