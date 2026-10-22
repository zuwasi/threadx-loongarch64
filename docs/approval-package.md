# Approval Package, Release 1.0.0-rc1

This page lists every item that needs a human decision before a final
1.0.0 release. Nothing on this page has been approved unless the item's own
record shows a name, date and decision. Approvals are confirmed by signed
tags (`approval/<item>-rN`), as described in `docs/misra/gep.md` section 5.3.

## 1. Documents

| Document | Technical lead | Safety manager |
|---|---|---|
| [Safety plan](safety/safety-plan.md) | Pending | Approved 2026-10-19 |
| [Requirements](safety/requirements.md) | Pending | Approved 2026-10-19 (M2 baseline) |
| [Tool classification](safety/tools.md) | Pending | Approved 2026-10-19 |
| [Design](safety/design.md) | Pending | Pending |
| [Compiler behaviour](safety/compiler-behaviour.md) | Pending | Pending |
| [Verification report](verification/verification-report.md) | | Pending |
| [Coverage report](verification/coverage.md) | Pending | Pending |
| [Safety manual](safety/safety-manual.md) | Pending | Pending |

## 2. MISRA deviations

| Record | Status |
|---|---|
| DEV-001 | Conditionally approved by the safety manager (2026-10-19); technical lead approval pending; conditions 1 to 3 met, condition 4 open ([record](misra/deviations/DEV-001.md)) |
| DEV-002 to DEV-015 | Draft, approval pending |

## 3. Reviews

| Review | Record | Status |
|---|---|---|
| Independent code review (port and BSP, including assembly) | [docs/reviews/review-log.md](reviews/review-log.md) | Not performed |
| Independent design review | [docs/reviews/review-log.md](reviews/review-log.md) | Not performed |

## 4. Open technical items

1. Statement, branch and MC/DC coverage (`docs/verification/coverage.md`).
2. Real-hardware validation.
3. Fault-injection tests for the defensive paths, or approved
   justifications.
