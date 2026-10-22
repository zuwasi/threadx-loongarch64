# ThreadX Common Kernel MISRA C:2025 Gap Analysis

| | |
|---|---|
| Document ID | ESL-TXLA64-KGA |
| Revision | r1, 2026-10-21 |
| Scope | `external/threadx/common/` C source |
| Tool | Parasoft C/C++test Standard 2026.1.3, MISRA C:2025 rule map 9.5.0 |
| Input report | [`reports/product/report.sarif`](reports/product/report.sarif) and [`report.html`](reports/product/report.html) |
| Source baseline | `c7e62ff`; ThreadX submodule pinned to `v6.5.2.202603_rel` |

## 1. Purpose and boundary

This report records the MISRA C:2025 findings produced for the pre-existing
ThreadX common kernel. The project plan makes this a report-only activity:
the submodule is not modified, and this report does not claim that ThreadX
common code complies with MISRA C:2025. Findings are static-analysis results,
not confirmed defects or safety failures.

The LoongArch64 port and QEMU BSP remain the project compliance scope. Their
results and deviations are reported separately in the
[Guideline Compliance Summary](reports/guideline-compliance-summary.md).

## 2. Method

The product Parasoft analysis builds the kernel and port together with the
Release configuration. This gap analysis filters the SARIF results to source
locations under `external/threadx/common/`. The ThreadX source and submodule
revision were not changed for this analysis.

Counts below are raw checker results. Different checker variants can report
the same source location, so the counts are not a count of unique defects.
Parasoft `error` and `warning` levels are tool severities; MISRA categories
are reported separately and are not safety classifications.

## 3. Result

| Measure | Result |
|---|---:|
| Raw findings in ThreadX common source | 613 |
| C source files with findings | 178 |
| Parasoft `error` results | 336 |
| Parasoft `warning` results | 277 |
| Distinct violated guidelines | 19 of 219 |
| Violated Mandatory guidelines | 0 |
| Violated Required guidelines | 11 |
| Violated Advisory guidelines | 8 |

### Findings by violated guideline

| Guideline | MISRA category | Raw findings | Initial triage priority |
|---|---|---:|---|
| Rule 5.10 | Required | 229 | High: reserved identifiers are pervasive in ThreadX's internal API; review representative uses and the compiler/library boundary before proposing any upstream change. |
| Rule 8.13 | Advisory | 113 | Medium: inspect pointer parameters in context; distinguish read-only inputs from output or in/out parameters. |
| Rule 13.3 | Advisory | 105 | Medium: review compound increment/decrement expressions for readability and side-effect ordering. |
| Dir 4.4 | Advisory | 40 | Medium: inspect reported commented-out code and establish whether it is configuration scaffolding or stale code. |
| Dir 4.1 | Required | 25 | High: review run-time risk findings individually, especially pointer comparisons and arithmetic. |
| Rule 1.1 | Required | 24 | High: the C90/C99 checker variants overlap; confirm the implementation translation limits and target ABI implications. |
| Rule 18.3 | Required | 16 | High: review comparisons of pointers that may refer to different objects. |
| Rule 14.3 | Required | 15 | Medium: inspect invariant controlling expressions and their build-time configuration. |
| Rule 10.3 | Required | 9 | Medium: assess narrowing and essential-type conversions against ThreadX's configured 32-bit `ULONG` model. |
| Rule 18.2 | Required | 8 | High: review pointer subtractions and the array/object bounds assumptions. |
| Rule 17.8 | Advisory | 6 | Low: review modified parameters for clarity and maintainability. |
| Rule 8.7 | Advisory | 6 | Low: evaluate external linkage findings in the full library/link configuration. |
| Rule 2.8 | Advisory | 4 | Low: identify unused external object definitions and their configuration guards. |
| Rule 17.7 | Required | 4 | Medium: verify that ignored return values do not carry required error information. |
| Rule 17.12 | Advisory | 3 | Low: review function identifier use. |
| Rule 10.7 | Required | 2 | Medium: review composite-expression conversions. |
| Rule 2.2 | Required | 2 | Medium: confirm that reported code is unreachable or dead in all supported configurations. |
| Rule 10.4 | Required | 1 | Low: review the reported mixed essential-type operands. |
| Rule 8.4 | Required | 1 | Medium: verify compatible declarations at the reported definition. |

The 336 Required results and 277 Advisory results reconcile with the Parasoft
severity totals in this baseline. The most frequent individual checkers are
Rule 5.10-a (209), Rule 8.13-a (113), Rule 13.3-a (105), Dir 4.4-b (40), and
Dir 4.1-j (16). Rule 5.10 has 229 total results across checker variants a, b,
and c; Rule 1.1 has 24 across C90 and C99 variants.

## 4. Disposition and follow-up

1. Do not alter `external/threadx/` as part of this port project.
2. Treat Required findings as candidates for focused technical review, not as
   approved deviations for the port. If a future upstream change is proposed,
   evaluate compatibility, API impact, test impact, and the exact ThreadX
   release before submitting it upstream.
3. Treat Advisory findings as maintainability opportunities. Prioritize
   representative samples from Rules 8.13 and 13.3 and Dir 4.4 before drawing
   conclusions about the full set.
4. Re-run the analysis if the ThreadX submodule revision, Parasoft version,
   rule map, or product build configuration changes.

No finding in this report has been individually reviewed or accepted as a
deviation. The 0 Mandatory findings are an analysis result for this source
baseline, not a general claim about other ThreadX versions or build options.
