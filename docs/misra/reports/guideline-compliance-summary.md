# Guideline Compliance Summary - MISRA C:2025

| | |
|---|---|
| Project | Eclipse ThreadX port for LoongArch64 (ESL) |
| Standard | MISRA C:2025, compliance per MISRA Compliance:2020 |
| Scope | `ports/loongarch64/` and `bsp/` C sources and headers (GEP section 2) |
| Analysis | Parasoft C/C++test Standard 2026.1.3, configuration under version control (`tools/parasoft/ESL_MISRA_C_2025.properties`) |
| Report | [product/report.sarif](product/report.sarif), [product/report.html](product/report.html) |
| Baseline | M5 working tree; the commit is recorded here when the baseline is committed |

## 1. Result for the compliance scope

All 219 guidelines in the GRP were evaluated.  206 are compliant in the compliance scope without deviation, 12 are complied with
through approved-track deviation records, and 1 is disapplied by the GRP.

The product-scope analysis reports no violation in `ports/loongarch64/` or
`bsp/` that is not covered by a deviation record.

Findings under `external/threadx/` (613 in the product-scope report) are the
input to the separate kernel gap analysis (M7); the upstream test sources in
the `tests/` report are analysed but reported separately (GEP scope table).

## 2. Deviations

| Guideline | Category | Records |
|---|---|---|
| Dir 4.1 Run-time failures shall be minimized (Required) | Required | DEV-014 |
| Dir 4.6 typedefs that indicate size and signedness should be used in place of the basic integer types (Advisory) | Advisory | DEV-003, DEV-008 |
| Dir 4.9 A function should be used in preference to a function-like macro where they are interchangeable (Advisory) | Advisory | DEV-004 |
| Rule 1.1 The program shall contain no violations of the standard C syntax and constraints, and shall not exceed the implementation's translation limits (Required) | Required | DEV-005 |
| Rule 2.3 A project should not contain unused type declarations (Advisory) | Advisory | DEV-010 |
| Rule 2.8 A project should not contain unused object definitions (Advisory) | Advisory | DEV-013 |
| Rule 5.10 A reserved identifier or reserved macro name shall not be declared (Required) | Required | DEV-001, DEV-002 |
| Rule 8.6 An identifier with external linkage shall have exactly one external definition (Required) | Required | DEV-006, DEV-013 |
| Rule 8.7 Functions and objects should not be defined with external linkage if they are referenced in only one translation unit (Advisory) | Advisory | DEV-011 |
| Rule 11.4 A conversion shall not be performed between a pointer to object and an arithmetic type (Required) | Required | DEV-007 |
| Rule 11.5 A conversion should not be performed from pointer to void into pointer to object (Advisory) | Advisory | DEV-009 |
| Rule 20.5 #undef should not be used (Advisory) | Advisory | DEV-012 |

All deviation records are in [deviations/](../deviations/); their approval
status is recorded in each record.  The compliance claim of this baseline
becomes effective when the record approvals are signed (GEP section 5.3).

## 3. Guideline-by-guideline result

| Guideline | MISRA category | Project category | Checkers | Result |
|---|---|---|---|---|
| Dir 4.1 | Required | Required | a, b, c, d, e, g, h, i, j, k, l, m | Deviations (DEV-014) |
| Dir 4.2 | Advisory | Advisory | a | Compliant |
| Dir 4.3 | Required | Required | a | Compliant |
| Dir 4.4 | Advisory | Advisory | a, b | Compliant |
| Dir 4.5 | Advisory | Advisory | a | Compliant |
| Dir 4.6 | Advisory | Advisory | a, b, c | Deviations (DEV-003, DEV-008) |
| Dir 4.7 | Required | Required | a, b | Compliant |
| Dir 4.8 | Advisory | Advisory | a | Compliant |
| Dir 4.9 | Advisory | Advisory | a | Deviations (DEV-004) |
| Dir 4.10 | Required | Required | a | Compliant |
| Dir 4.11 | Required | Required | a | Compliant |
| Dir 4.12 | Required | Required | a | Compliant |
| Dir 4.13 | Advisory | Advisory | a, b, c, d, e, f | Compliant |
| Dir 4.14 | Required | Required | a, b, c, d, e, f, g, h, i, j, k, l | Compliant |
| Dir 4.15 | Required | Required | a | Compliant |
| Dir 5.1 | Required | Required | a, b, c | Compliant |
| Dir 5.2 | Required | Required | a | Compliant |
| Dir 5.3 | Required | Required | a | Compliant |
| Rule 1.1 | Required | Required | a, b, c, d | Deviations (DEV-005) |
| Rule 1.3 | Required | Required | a, b, c, d, e, f, g, h, i, j, k, l, m, n, o | Compliant |
| Rule 1.4 | Required | Required | j, k, l, m, n | Compliant |
| Rule 1.5 | Required | Required | a, b, c, d, e, f, g | Compliant |
| Rule 2.1 | Required | Required | a, b, c, d, e, f, g, h | Compliant |
| Rule 2.2 | Required | Required | a, b | Compliant |
| Rule 2.3 | Advisory | Advisory | a, b | Deviations (DEV-010) |
| Rule 2.4 | Advisory | Advisory | a, b | Compliant |
| Rule 2.5 | Advisory | Advisory | a | Compliant |
| Rule 2.6 | Advisory | Advisory | a | Compliant |
| Rule 2.7 | Advisory | Advisory | a | Compliant |
| Rule 2.8 | Advisory | Advisory | a, b, c | Deviations (DEV-013) |
| Rule 3.1 | Required | Required | a, b, c | Compliant |
| Rule 3.2 | Required | Required | a | Compliant |
| Rule 4.1 | Required | Required | a | Compliant |
| Rule 4.2 | Advisory | Advisory | a | Compliant |
| Rule 5.1 | Required | Required | a | Compliant |
| Rule 5.2 | Required | Required | a, b, c, d | Compliant |
| Rule 5.3 | Required | Required | a, b | Compliant |
| Rule 5.4 | Required | Required | a, b, c, d | Compliant |
| Rule 5.5 | Required | Required | a, b | Compliant |
| Rule 5.6 | Required | Required | a | Compliant |
| Rule 5.7 | Required | Required | a | Compliant |
| Rule 5.8 | Required | Required | a, b | Compliant |
| Rule 5.9 | Advisory | Advisory | a, b | Compliant |
| Rule 5.10 | Required | Required | a, b, c | Deviations (DEV-001, DEV-002) |
| Rule 6.1 | Required | Required | a | Compliant |
| Rule 6.2 | Required | Required | a | Compliant |
| Rule 6.3 | Required | Required | a | Compliant |
| Rule 7.1 | Required | Required | a | Compliant |
| Rule 7.2 | Required | Required | a | Compliant |
| Rule 7.3 | Required | Required | a | Compliant |
| Rule 7.4 | Required | Required | a | Compliant |
| Rule 7.5 | Mandatory | Mandatory | a | Compliant |
| Rule 7.6 | Required | Required | a | Compliant |
| Rule 8.1 | Required | Required | a, b | Compliant |
| Rule 8.2 | Required | Required | a, b, c | Compliant |
| Rule 8.3 | Required | Required | a, b, c | Compliant |
| Rule 8.4 | Required | Required | a, b | Compliant |
| Rule 8.5 | Required | Required | a | Compliant |
| Rule 8.6 | Required | Required | a, b | Deviations (DEV-006, DEV-013) |
| Rule 8.7 | Advisory | Advisory | a | Deviations (DEV-011) |
| Rule 8.8 | Required | Required | a | Compliant |
| Rule 8.9 | Advisory | Advisory | a | Compliant |
| Rule 8.10 | Required | Required | a | Compliant |
| Rule 8.11 | Advisory | Advisory | a | Compliant |
| Rule 8.12 | Required | Required | a | Compliant |
| Rule 8.13 | Advisory | Advisory | a, b | Compliant |
| Rule 8.14 | Required | Required | a | Compliant |
| Rule 8.15 | Required | Required | a, b | Compliant |
| Rule 8.16 | Advisory | Advisory | a | Compliant |
| Rule 8.17 | Advisory | Advisory | a | Compliant |
| Rule 8.18 | Required | Required | a | Compliant |
| Rule 8.19 | Advisory | Advisory | a, b, c, d | Compliant |
| Rule 9.1 | Mandatory | Mandatory | a | Compliant |
| Rule 9.2 | Required | Required | a | Compliant |
| Rule 9.3 | Required | Required | a | Compliant |
| Rule 9.4 | Required | Required | a | Compliant |
| Rule 9.5 | Required | Required | a | Compliant |
| Rule 9.6 | Required | Required | a | Compliant |
| Rule 9.7 | Mandatory | Mandatory | a | Compliant |
| Rule 10.1 | Required | Required | a, b, c, d, e, f, g, h | Compliant |
| Rule 10.2 | Required | Required | a | Compliant |
| Rule 10.3 | Required | Required | a, b | Compliant |
| Rule 10.4 | Required | Required | a, b | Compliant |
| Rule 10.5 | Advisory | Advisory | a, b, c | Compliant |
| Rule 10.6 | Required | Required | a | Compliant |
| Rule 10.7 | Required | Required | a, b | Compliant |
| Rule 10.8 | Required | Required | a | Compliant |
| Rule 11.1 | Required | Required | a, b | Compliant |
| Rule 11.2 | Required | Required | a | Compliant |
| Rule 11.3 | Required | Required | a | Compliant |
| Rule 11.4 | Required | Required | b, c | Deviations (DEV-007) |
| Rule 11.5 | Advisory | Advisory | b | Deviations (DEV-009) |
| Rule 11.6 | Required | Required | b, c | Compliant |
| Rule 11.8 | Required | Required | a | Compliant |
| Rule 11.9 | Required | Required | a, b | Compliant |
| Rule 11.10 | Required | Required | a | Compliant |
| Rule 11.11 | Required | Required | a | Compliant |
| Rule 12.1 | Advisory | Advisory | a, c | Compliant |
| Rule 12.2 | Required | Required | a, b | Compliant |
| Rule 12.3 | Advisory | Advisory | a | Compliant |
| Rule 12.4 | Advisory | Advisory | a, b | Compliant |
| Rule 12.5 | Mandatory | Mandatory | a | Compliant |
| Rule 12.6 | Required | Required | a | Compliant |
| Rule 13.1 | Required | Required | a | Compliant |
| Rule 13.2 | Required | Required | a, b, c, d, e, f, g, h | Compliant |
| Rule 13.3 | Advisory | Advisory | a | Compliant |
| Rule 13.4 | Advisory | Advisory | a | Compliant |
| Rule 13.5 | Required | Required | a | Compliant |
| Rule 13.6 | Required | Required | a, b, c | Compliant |
| Rule 14.1 | Required | Required | a, b | Compliant |
| Rule 14.2 | Required | Required | a, b, c, d | Compliant |
| Rule 14.3 | Required | Required | ac | Compliant |
| Rule 14.4 | Required | Required | a | Compliant |
| Rule 15.1 | Advisory | Advisory | a | Compliant |
| Rule 15.2 | Required | Required | a | Compliant |
| Rule 15.3 | Required | Required | a | Compliant |
| Rule 15.4 | Advisory | Advisory | a | Compliant |
| Rule 15.5 | Disapplied | Disapplied | a | Disapplied (Disapplied by MISRA C:2025 (GRP section 2)) |
| Rule 15.6 | Required | Required | a, b | Compliant |
| Rule 15.7 | Required | Required | a | Compliant |
| Rule 16.1 | Required | Required | a, b, e, f, g, h, i | Compliant |
| Rule 16.2 | Required | Required | a | Compliant |
| Rule 16.3 | Required | Required | c | Compliant |
| Rule 16.4 | Required | Required | a, b | Compliant |
| Rule 16.5 | Required | Required | a | Compliant |
| Rule 16.6 | Required | Required | a | Compliant |
| Rule 16.7 | Required | Required | a, b | Compliant |
| Rule 17.1 | Required | Required | a, b, c | Compliant |
| Rule 17.2 | Required | Required | b | Compliant |
| Rule 17.3 | Mandatory | Mandatory | a | Compliant |
| Rule 17.4 | Mandatory | Mandatory | a, b | Compliant |
| Rule 17.5 | Required | Required | a | Compliant |
| Rule 17.7 | Required | Required | a, b | Compliant |
| Rule 17.8 | Advisory | Advisory | a | Compliant |
| Rule 17.9 | Mandatory | Mandatory | a | Compliant |
| Rule 17.10 | Required | Required | a | Compliant |
| Rule 17.11 | Advisory | Advisory | a | Compliant |
| Rule 17.12 | Advisory | Advisory | a | Compliant |
| Rule 17.13 | Required | Required | a | Compliant |
| Rule 18.1 | Required | Required | a, b, c | Compliant |
| Rule 18.2 | Required | Required | a | Compliant |
| Rule 18.3 | Required | Required | a | Compliant |
| Rule 18.4 | Advisory | Advisory | a | Compliant |
| Rule 18.5 | Advisory | Advisory | a | Compliant |
| Rule 18.6 | Required | Required | a, b, c | Compliant |
| Rule 18.7 | Required | Required | a | Compliant |
| Rule 18.8 | Required | Required | a | Compliant |
| Rule 18.9 | Required | Required | a | Compliant |
| Rule 18.10 | Mandatory | Mandatory | a | Compliant |
| Rule 19.1 | Mandatory | Mandatory | a, b, c | Compliant |
| Rule 19.2 | Advisory | Advisory | a | Compliant |
| Rule 19.3 | Required | Required | a | Compliant |
| Rule 20.1 | Advisory | Advisory | a | Compliant |
| Rule 20.2 | Required | Required | a, b | Compliant |
| Rule 20.3 | Required | Required | a | Compliant |
| Rule 20.4 | Required | Required | a, b | Compliant |
| Rule 20.5 | Advisory | Advisory | a | Deviations (DEV-012) |
| Rule 20.6 | Required | Required | a | Compliant |
| Rule 20.7 | Required | Required | b | Compliant |
| Rule 20.8 | Required | Required | a | Compliant |
| Rule 20.9 | Required | Required | b | Compliant |
| Rule 20.10 | Advisory | Advisory | a | Compliant |
| Rule 20.11 | Required | Required | a | Compliant |
| Rule 20.12 | Required | Required | a | Compliant |
| Rule 20.13 | Required | Required | a | Compliant |
| Rule 20.14 | Required | Required | a | Compliant |
| Rule 20.15 | Required | Required | a, b, c, d | Compliant |
| Rule 21.3 | Required | Required | a | Compliant |
| Rule 21.4 | Required | Required | a, b | Compliant |
| Rule 21.5 | Required | Required | a, b | Compliant |
| Rule 21.6 | Required | Required | a | Compliant |
| Rule 21.7 | Required | Required | a | Compliant |
| Rule 21.8 | Required | Required | a, b, c, d | Compliant |
| Rule 21.9 | Required | Required | a | Compliant |
| Rule 21.10 | Required | Required | a, b, c | Compliant |
| Rule 21.11 | Advisory | Advisory | a | Compliant |
| Rule 21.12 | Required | Required | a, b | Compliant |
| Rule 21.13 | Mandatory | Mandatory | a | Compliant |
| Rule 21.14 | Required | Required | a | Compliant |
| Rule 21.15 | Required | Required | a | Compliant |
| Rule 21.16 | Required | Required | a | Compliant |
| Rule 21.17 | Mandatory | Mandatory | a, b | Compliant |
| Rule 21.18 | Mandatory | Mandatory | a | Compliant |
| Rule 21.19 | Mandatory | Mandatory | a, b | Compliant |
| Rule 21.20 | Mandatory | Mandatory | a | Compliant |
| Rule 21.21 | Required | Required | a | Compliant |
| Rule 21.22 | Mandatory | Mandatory | a | Compliant |
| Rule 21.23 | Required | Required | a | Compliant |
| Rule 21.24 | Required | Required | a | Compliant |
| Rule 21.25 | Required | Required | a | Compliant |
| Rule 21.26 | Required | Required | a | Compliant |
| Rule 22.1 | Required | Required | a | Compliant |
| Rule 22.2 | Mandatory | Mandatory | a, b | Compliant |
| Rule 22.3 | Required | Required | a | Compliant |
| Rule 22.4 | Mandatory | Mandatory | a | Compliant |
| Rule 22.5 | Mandatory | Mandatory | a, b | Compliant |
| Rule 22.6 | Mandatory | Mandatory | a | Compliant |
| Rule 22.7 | Required | Required | a | Compliant |
| Rule 22.8 | Required | Required | a | Compliant |
| Rule 22.9 | Required | Required | a | Compliant |
| Rule 22.10 | Required | Required | a | Compliant |
| Rule 22.11 | Required | Required | a | Compliant |
| Rule 22.12 | Mandatory | Mandatory | a | Compliant |
| Rule 22.13 | Required | Required | a | Compliant |
| Rule 22.14 | Mandatory | Mandatory | a, b | Compliant |
| Rule 22.15 | Required | Required | a | Compliant |
| Rule 22.16 | Required | Required | a | Compliant |
| Rule 22.17 | Required | Required | a | Compliant |
| Rule 22.18 | Required | Required | a | Compliant |
| Rule 22.19 | Required | Required | a | Compliant |
| Rule 22.20 | Mandatory | Mandatory | a | Compliant |
| Rule 23.1 | Advisory | Advisory | a, b | Compliant |
| Rule 23.2 | Required | Required | a | Compliant |
| Rule 23.3 | Advisory | Advisory | a | Compliant |
| Rule 23.4 | Required | Required | a | Compliant |
| Rule 23.5 | Advisory | Advisory | a | Compliant |
| Rule 23.6 | Required | Required | a, b | Compliant |
| Rule 23.7 | Advisory | Advisory | a | Compliant |
| Rule 23.8 | Required | Required | a | Compliant |
