# ThreadX for LoongArch64: documentation

Release candidate **1.0.0-rc1** (2026-10-22), pre-release tag `v1.0.0-rc1`.

An Eclipse ThreadX port for the LoongArch64 (LA64) architecture with a QEMU
`virt` board support package, developed to an IEC 61508-3 SC 3 software
process. All automated verification passes; the human approvals, independent
reviews and full structural coverage listed in the
[approval package](approval-package.md) are still open. The element is not
certified.

## Safety and process

| Document | |
|---|---|
| Project plan | [project-plan.md](project-plan.md) |
| Software safety plan | [safety/safety-plan.md](safety/safety-plan.md) |
| Safety requirements | [safety/requirements.md](safety/requirements.md) |
| Design description | [safety/design.md](safety/design.md) |
| Compiler behaviour (Dir 1.1) | [safety/compiler-behaviour.md](safety/compiler-behaviour.md) |
| Tool classification | [safety/tools.md](safety/tools.md) |
| Verification plan | [safety/verification.md](safety/verification.md) |
| Traceability matrix (generated) | [safety/traceability.md](safety/traceability.md) |
| Safety manual | [safety/safety-manual.md](safety/safety-manual.md) |

## Verification

| Document | |
|---|---|
| Verification report | [verification/verification-report.md](verification/verification-report.md) |
| ThreadX regression results | [verification/regression.md](verification/regression.md) |
| Structural coverage | [verification/coverage.md](verification/coverage.md) |

## MISRA C:2025 compliance

| Document | |
|---|---|
| Guideline Enforcement Plan | [misra/gep.md](misra/gep.md) |
| Guideline Re-categorization Plan | [misra/grp.md](misra/grp.md) |
| Kernel gap analysis (report only) | [misra/kernel-gap-analysis.md](misra/kernel-gap-analysis.md) |
| Guideline compliance summary | [misra/reports/guideline-compliance-summary.md](misra/reports/guideline-compliance-summary.md) |
| Compliance dashboard | [misra/reports/dashboard.html](misra/reports/dashboard.html) |
| Reports, how to regenerate | [misra/reports/README.md](misra/reports/README.md) |

Deviation records, one per justified departure:

| | | | | |
|---|---|---|---|---|
| [DEV-001](misra/deviations/DEV-001.md) | [DEV-002](misra/deviations/DEV-002.md) | [DEV-003](misra/deviations/DEV-003.md) | [DEV-004](misra/deviations/DEV-004.md) | [DEV-005](misra/deviations/DEV-005.md) |
| [DEV-006](misra/deviations/DEV-006.md) | [DEV-007](misra/deviations/DEV-007.md) | [DEV-008](misra/deviations/DEV-008.md) | [DEV-009](misra/deviations/DEV-009.md) | [DEV-010](misra/deviations/DEV-010.md) |
| [DEV-011](misra/deviations/DEV-011.md) | [DEV-012](misra/deviations/DEV-012.md) | [DEV-013](misra/deviations/DEV-013.md) | [DEV-014](misra/deviations/DEV-014.md) | [DEV-015](misra/deviations/DEV-015.md) |

## Release

| Document | |
|---|---|
| Release notes | [release-notes.md](release-notes.md) |
| Approval package (open decisions) | [approval-package.md](approval-package.md) |
| Review log | [reviews/review-log.md](reviews/review-log.md) |
