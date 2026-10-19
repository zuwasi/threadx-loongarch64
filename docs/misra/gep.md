# MISRA C:2025 Guideline Enforcement Plan

| | |
|---|---|
| Document ID | ESL-TXLA64-GEP |
| Project | Eclipse ThreadX port for LoongArch64 |
| Applies to | Port (`ports/loongarch64/`) and BSP (`bsp/`) C and header files |
| Standard | MISRA C:2025, compliance per MISRA Compliance:2020 |
| Analysis tool | Parasoft C/C++test Standard 2026.1.3, configuration "MISRA C 2025" (Automotive Pack, rule map 9.5.0) |
| Owner | Engineering Software Lab Israel (ESL) |

## 1. Purpose

This plan defines how each MISRA C:2025 guideline is enforced for this project,
how violations are handled, and how compliance is reported. It is part of the
IEC 61508-3 evidence for the coding standard (Part 3, Table A.3 and 7.4.4.13).

## 2. Scope

| In scope | Out of scope |
|---|---|
| All C sources and headers under `ports/loongarch64/` and `bsp/` | Eclipse ThreadX common code in `external/threadx/` (covered by a separate gap analysis report) |
| Test harness code written by ESL, analysed but reported separately | Assembly files (`*.S`), covered by Dir 4.2 documentation and code review |

## 3. Guideline categories and deviation policy

The project applies the following policy. It is consistent with MISRA
Compliance:2020.

| MISRA category | Project policy |
|---|---|
| **Mandatory** | Must be complied with. No deviation is permitted. |
| **Required** | Must be complied with, unless a formal deviation record is approved. The record must show, with technical evidence, that the deviation does not introduce a risk to the safety function. |
| **Advisory** | Applied where it does not prevent suitable, efficient code. An advisory guideline may be deviated with a documented rationale, or disapplied project-wide through the Guideline Re-categorization Plan (GRP). |
| **Disapplied** | Not enforced. |

The project categories of all guidelines are recorded in the GRP
([grp.md](grp.md)). A guideline can be raised in category (for example,
Advisory to Required) but never lowered below its MISRA category, except
Advisory to Disapplied.

## 4. Enforcement method

| Guideline type | Enforcement |
|---|---|
| Decidable rules covered by a Parasoft checker | Parasoft static analysis, run on every change set before commit and before each release |
| Undecidable rules (data flow) | Parasoft flow analysis (BD-* checkers in the configuration) plus code review |
| Directives and rules without full tool coverage | Code review against the review checklist, recorded in the review log |
| Dir 4.2 / Dir 4.3 (assembly) | Assembly is confined to `*.S` files with documented interfaces; no inline assembly in C |

The project test configuration is derived from the built-in "MISRA C 2025"
configuration. Differences are limited to:

- checkers of guidelines that the GRP marks as Disapplied are disabled;
- no checker of a Mandatory or Required guideline may be disabled.

The derived configuration is kept under version control in `tools/parasoft/`.

## 5. Deviations

### 5.1 Deviation record

Each deviation of a Required or Advisory guideline has a record in
`docs/misra/deviations/DEV-nnn.md`, using the
[deviation template](deviation-template.md). A Required deviation record must
include:

1. the guideline and the Parasoft checker IDs;
2. the exact code locations covered;
3. the reason compliance is not possible or not reasonable;
4. the risk analysis: what failure the guideline protects against, why that
   failure cannot occur here, and the evidence (tests, analysis, review);
5. any additional measures that compensate for the deviation;
6. reviewer and approver.

### 5.2 Marking in source code

Each deviating location carries a Parasoft suppression comment that names the
deviation record:

```c
/* parasoft-suppress MISRAC2025-RULE_5_10-a "DEV-001" */
```

A suppression without a deviation ID is a process violation. The compliance
report lists all suppressions with their deviation IDs.

### 5.3 Approval

| Category | Approver |
|---|---|
| Required | Safety manager and technical lead, both required |
| Advisory | Technical lead |

Approvals are recorded in the deviation record as name, role, date and
decision. Each approval is confirmed by a cryptographically signed git tag
(`approval/DEV-nnn-rN`) on the commit that contains the approved record.
Handwritten signature images are not stored in the repository.

The safety manager and the technical lead must be different people.

## 6. Compliance reporting

For each baseline the project publishes, in `docs/misra/reports/`:

- the Parasoft HTML and SARIF reports;
- a Guideline Compliance Summary (GCS): for every guideline, one of
  *Compliant*, *Deviations* (with IDs), or *Disapplied*.

A baseline may claim MISRA C:2025 compliance only if:

- there are no violations of Mandatory guidelines;
- every violation of a Required or Advisory guideline is covered by an approved
  deviation record or by the GRP;
- the analysis ran on the exact sources of the baseline, with the configuration
  under version control.

## 7. Toolchain conditions

| Item | Value |
|---|---|
| Language | ISO C99 (`-std=c99`), GNU extensions not used in C sources |
| Compiler | GCC 14 `loongarch64-linux-gnu`, freestanding (`-ffreestanding -nostdinc`) |
| Implementation-defined behaviour | Documented in `docs/safety/compiler-behaviour.md` (Dir 1.1) |

## 8. References

- MISRA C:2025, Guidelines for the use of the C language in critical systems
- MISRA Compliance:2020, Achieving compliance with MISRA Coding Guidelines
- IEC 61508-3:2010, Part 3: Software requirements
